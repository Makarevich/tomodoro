#ifndef TIMER_H
#define TIMER_H

#include <QTimer>
#include <QMenu>
#include <QSystemTrayIcon>


class PieView;

class Timer : public QObject
{
    Q_OBJECT

private:

    QTimer              m_timer;
    QMenu               m_menu;
    QSystemTrayIcon     m_icon;
    QWidget             m_parent_holder;

    QAction*            act_start;
    QAction*            act_stop;

    int         m_counter;
    int         m_total;


    PieView*    m_view;

    void show_hide_actions();

private slots:
    void timeout();

    void action_start();
    void action_stop();
    void action_exit();

    void view_context_request(QPoint);


public:
    explicit Timer(QObject *parent = 0);
    
signals:
    void tick(int current, int total);

public slots:
    
};

#endif // TIMER_H
