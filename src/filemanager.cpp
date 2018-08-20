#include "mainwindow.h"

/* Reads a configuration file*/
void FileManager::load_auto(QString name, Player* mediaplayer)
{
    QString s, t, u;
    int a, b, c;
    Task new_task;

    QStringList st;
    int accumulator = 0;

    //got from arguments, instead of aut.atu
    if (name == "") name = "aut.aut";

    f = new QFile (name);

    if (!f->exists())
    {
        // react
    }
    if(!f->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        //show error of opened file
    }
    else{
     //first reads the "time interval" constants
      QTextStream in (f);
      in >> a >> b >> c;
      mediaplayer->set_break_time(b);
      mediaplayer->set_ult_break_time(c);
      mediaplayer->set_pomodoro_time(a);
      t = in.readLine(); //clean line
      t = in.readLine();
      u = in.readLine();

    //reads the whole list of tasks
      while(!in.atEnd())
        { s = in.readLine();
          s.replace(" ", "");
          st = s.split(QRegExp("[(){}]"), QString::SkipEmptyParts);

          if (st[0] != 0)
           {accumulator += st[0].toInt();
            new_task.sessions = accumulator;
            new_task.name = st[1];
            new_task.done = false;
            if(st.size() > 2) new_task.playlist = st[2];
            if(st.size() > 3) new_task.script = st[3];
            mediaplayer->add_task(new_task);
           }
        }
     //reads to execute scripts & playlists
        mediaplayer->set_break_script(u);
        mediaplayer->set_break_play(t);

    f->close();
    }
    delete f;
    mediaplayer->clock_start();
}
