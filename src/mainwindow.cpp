#include "mainwindow.h"
#include "ui_mainwindow.h"
using namespace std;

/* main class*/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    mediaplayer = new Player(this);

    filemanager = new FileManager();
    //first_time = !QFile::exists("log/log") | !QFile::exists("log/today.log") | !QFile::exists("log/yesterday.log");
}

MainWindow::~MainWindow()
{   log_save();
    delete mediaplayer;

}

/* write logs, called when exiting*/
void MainWindow::log_save()
{
  QFile f ("log/log");
  QFile t ("log/today.log");
  QFile y ("log/yesterday.log");


  //saved something before
  if(t.exists())
   { if (t.open(QIODevice::ReadOnly))
    {
    //reads most recent today.log
     QTextStream in (&t);
     int t_day, t_month;
     in >> t_day >> t_month;
    //if there is already an entry with the same date, it sums the counter
     if((t_day == day) && (t_month == month)) //today
         {int i = 0, aux;
          while(!in.atEnd())
            {in >> aux;
             i += aux;
            }
            mediaplayer->set_number(mediaplayer->get_number()+i);
          t.close();
          if (t.open(QIODevice::WriteOnly))
            { QTextStream out (&t);
             out << day <<  " " << month << " " << mediaplayer->get_number();
             t.close();
            }
         }
     // we use yesterday.log instead
     else
      { y.rename("old");
        t.rename("yesterday.log");

        int y_day, y_month, y_number;
        QFile o("old");
        if (o.open(QIODevice::ReadOnly))
            {QTextStream in2 (&o);
            in2 >> y_day >> y_month >> y_number;
             y.close();
             y.remove();
             if (f.open(QIODevice::WriteOnly | QIODevice::Append))
                { QTextStream out (&f);
                   out << endl << y_day <<  " " << y_month << " " << y_number;
                   f.close();
                }
             //what to do to compare
            }
        QFile a("today.log");
        if (a.open(QIODevice::WriteOnly))
           { QTextStream out (&a);
              out << day <<  " " << month << " " << mediaplayer->get_number();
              f.close();
           }
      }
    }
   }

  else //used for the first time
    if (t.open(QIODevice::WriteOnly))
     { QTextStream out (&t);
      out << day <<  " " << month << " " << mediaplayer->get_number();
      t.close();
     }

}

/* shows program's window */
void MainWindow::show_player()
{
    show();
    setFixedHeight(500);
    setFixedWidth(300);
    mediaplayer->show();
    mediaplayer->setSizePolicy(this->sizePolicy());
    //mediaplayer->setSizeConstraint( QMainWindow::SetNoConstraint );
}

/* get program's main arguments*/
void MainWindow::read_args(QString s){ filemanager->load_auto(s, mediaplayer);}
