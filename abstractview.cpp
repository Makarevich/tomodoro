/*
 *  Copyright 2012 Yury Makarevich
 *
 *  This file is part of Tomodoro.
 *
 *  Tomodoro is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Tomodoro is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Tomodoro.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "abstractview.h"

#include "settings.h"

#include "customlabel.h"

#include <QMouseEvent>


#include <QApplication>
#include <QDesktopWidget>

AbstractView::AbstractView(QWidget *parent) :
    QWidget(parent, Qt::FramelessWindowHint| Qt::WindowStaysOnTopHint | Qt::Dialog),
    m_mouse_anchor(),
    m_win_anchor(),

    m_conf_buzz_dev(5),

    m_op_normal(50),
    m_op_focused(80),

    m_custom_label(new CustomLabel),

    m_current(0),
    m_total(100)

    //m_text("--:--")
{
    setAttribute(Qt::WA_TranslucentBackground);

    setContextMenuPolicy(Qt::CustomContextMenu);

    //update_settings();                // delegate that to descendants
}


void AbstractView::mousePressEvent(QMouseEvent* event) {
    if(event->button() == Qt::LeftButton) {
        m_mouse_anchor = event->globalPos();
    }
}

void AbstractView::mouseMoveEvent(QMouseEvent* event) {
    if(event->buttons() & Qt::LeftButton) {
        QPoint      n = event->globalPos();

        QPoint      win_pos = is_buzzing() ? m_win_anchor : pos();

        // shift the win position
        win_pos += n - m_mouse_anchor;

        // crop the movement
        QRect scr = QApplication::desktop()->screenGeometry();

        scr.adjust(0, 0, -width(), -height());

        if     (win_pos.x() < scr.left())   win_pos.setX(scr.left());
        else if(win_pos.x() > scr.right())  win_pos.setX(scr.right());

        if     (win_pos.y() < scr.top())    win_pos.setY(scr.top());
        else if(win_pos.y() > scr.bottom()) win_pos.setY(scr.bottom());


        if(is_buzzing()) {
            m_win_anchor = win_pos;
        } else {
            move(win_pos);
        }

        // memorize the mouse position

        m_mouse_anchor = n;
    }
}

void AbstractView::enterEvent(QEvent *) {
    setWindowOpacity(static_cast<double>(m_op_focused) / 100.0);
}


void AbstractView::leaveEvent(QEvent *) {
    setWindowOpacity(static_cast<double>(m_op_normal) / 100.0);
}


//////////////////////////////////////////

void AbstractView::tick(int current, int total) {

    //
    // Handle buzzing
    //

    const bool buzzing = is_buzzing();              // current state

    m_current = current;
    m_total = total;

    const bool about_to_buzz = is_buzzing();        // new state


    if(about_to_buzz) {
        // If we're gonna buzz, then offset the window

        if(buzzing) {
#define RAND_DEV        ((qrand() % (m_conf_buzz_dev * 2 + 1)) - m_conf_buzz_dev)
            move(m_win_anchor + QPoint(RAND_DEV, RAND_DEV));
#undef  RAND_DEV
        }else {
            m_win_anchor = pos();
        }

    } else if(buzzing) {
        // Buzzing has just turned off.
        move(m_win_anchor);
    }

    //
    // Handle repaint
    //

    // Update text and redraw unless we're in the middle of buzzing
    if( !(buzzing && about_to_buzz) ) {
        int remaining = m_total - m_current;
        QString text = QString("%1:%2")
                 .arg(QString::number(remaining / 60), 2, '0')
                 .arg(QString::number(remaining % 60), 2, '0');

        m_custom_label->setText(text);

        update();
    }
}

void AbstractView::update_settings() {
    Settings        s;

    m_conf_buzz_dev = s.view.buzz_dev;

    m_op_normal  = s.view.op_normal;
    m_op_focused = s.view.op_focused;

    // emulate leave event to set the opacity
    leaveEvent(NULL);

    // other view settings

    m_settings.flags        = s.view.main_dir;
    if(s.view.inverted != 0)
        m_settings.flags |= VIEW_INVERTED;

    m_settings.border  = s.defaultToText(this, (QColor)s.view.border);
    m_settings.filling = s.defaultToButton(this, (QColor)s.view.filling);

    // propagate the event to the custom label
    m_custom_label->update_settings();

    update();
}
