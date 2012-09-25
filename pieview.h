#ifndef PIEVIEW_H
#define PIEVIEW_H

#include <QPoint>
#include <QWidget>

class PieView : public QWidget
{
    Q_OBJECT

private:
    QPoint      m_mouse_anchor;     // mouse cursor anchor (for handling dragging)
    QPoint      m_win_anchor;       // buzzing anchor

    int         m_conf_buzz_dev;    // buzz deviation

    // NOTE: if (m_current == m_total) then we're in buzz mode

    int         m_current;          // seconds passed so far
    int         m_total;            // total seconds to pass

    QString     m_text;             // precalculated from current/total

    qreal       m_op_normal;        // normal opacity
    qreal       m_op_focused;       // focused opacity

    bool is_buzzing() const {
        return m_current == m_total;
    }

protected:
    void paintEvent ( QPaintEvent * event );

    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);

    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);


public:
    explicit PieView(QWidget *parent = 0);
    
signals:
    
public slots:
    void tick(int current, int total);
    void update_settings();

};

#endif // PIEVIEW_H
