#include "mainwindow.h"
#include "ui_mainwindow.h"


/* music, clock & interface class */
Player::Player(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setFixedSize(300, 500);

    ui->setupUi(this);
    QFont font;


    mediaplayer = new QMediaPlayer(this);
    playlist = new QMediaPlaylist();
    timer = new QTimer(this);
    fin.setHMS(0,0,0);

    ui->Phrase->setFont(font);
    font.setPointSize(32);
    //font.setBold(true);

    end = true; start = false; pomodoro = true; number = 0;

   //set background
    QString css = "QMainWindow {background: url(wallpapers/timemachine.jpg)0 0 0 0 stretch stretch; background-color: rgba(0,0,0,50%);}";
    setStyleSheet(css);
    statusBar()->setSizeGripEnabled(false);
    setWindowFlags(Qt::Widget | Qt::MSWindowsFixedSizeDialogHint);

   //set icons from external images
    QIcon icon ("wallpapers/clipboard.png");
    this->setWindowIcon(icon);

    QIcon icon2 ("wallpapers/pause.png");
    QIcon icon3 ("wallpapers/play.png");

    ui->Switch_Play->setIcon(icon2);

    QIcon icon4 ("wallpapers/restart.png");
    ui->Restart->setIcon(icon4);

    QIcon icon5 ("wallpapers/previous.png");
    ui->Previous->setIcon(icon5);

    QIcon icon6 ("wallpapers/next.png");
    ui->Next->setIcon(icon6);

  //set slots
    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
    //connect(ui->Pause, SIGNAL(pressed()), this, SLOT (pause()));
    //connect(ui->Play, SIGNAL(pressed()), this, SLOT (play()));
    connect(ui->Switch_Play, SIGNAL(pressed()), this, SLOT(switch_play_state()));
    connect(ui->Restart, SIGNAL(pressed()), this, SLOT (restart()));
    connect(ui->Next, SIGNAL(pressed()), this, SLOT(next()));
    connect(ui->Previous, SIGNAL(pressed()), this, SLOT(previous()));
    //connect(ui->Done, SIGNAL(pressed()), this, SLOT(done()));
}

Player::~Player(){delete ui;}

/* play slot (music & clock) */
void Player::play()
{   if(end)
       {end = false;
        clock_start();
        }
    else timer->start();

    mediaplayer->play();
    //playing it with spotify
    /*else
      {if (QFile::exists(spotify_play_script_path))
        { script_process = new QProcess();
          p->start(spotify_play_script_path);
        }
      }
    */

}

/* previous slot*/
void Player::previous() { playlist->previous();}

/* next slot */
void Player::next() {playlist->next();}

/* stop slot */
void Player::stop()
{   pause();
     pomodoro = !pomodoro;
     start = false;
     end = true;
}

/* restart slot */
void Player::restart()
{    pause();
     clock_start();
     timer->stop();
     mediaplayer->stop();
     play();
}

/* pause slot */
void Player::pause()
{    timer->stop();
     mediaplayer->pause();
}

/* continuous clock slot until it reaches 0*/
void Player::showTime()
{   if(time->secsTo(fin) == 0)
    { end = true;
      if (pomodoro) number++;

      //ui->Number->setText("Number of Pomodoros: " + QString::number(number));
      //ui->Number->setAlignment(Qt::AlignCenter);
    }
    else end = false;

  //changes the clock, substracting 1 second
    QString text = time->toString("mm:ss");
    time->setHMS(0,time->addSecs(-1).minute(),time->addSecs(-1).second());
    ui->Clock->display(text);
    timer->start(1000);
    if(end)
      { pause();
        pomodoro = !pomodoro;
      }
}


void Player::done()
{ task[task_iterator].done = true;
  task_iterator++;
  //number++
}

/* play & pause slot */
void Player::switch_play_state()
{ if(timer->isActive()) { pause(); }
  else { play(); }
}

/* slot that begins the clock & music */
void Player::clock_start()
{   if(pomodoro)
     {time = new QTime (0, pomodoro_time, 0);
      load_file();

     }
    else
        {if(number%4 == 0) time = new QTime (0, ult_break_time, 0);
         else time = new QTime(0, break_time, 0);
         load_playlist(-1);
        }

    showTime();
    end = false;
    timer->start();
}

/* load playlists from configuration file */
void Player::load_playlist(int pos)
{ playlist->clear();
  QString n;
  QUrl url;


  if (pos > -1 ) n = task[pos].playlist;
  else n = break_play;
  n.replace(" ", "");

  QDir dir(QCoreApplication::applicationDirPath());
  n = dir.absoluteFilePath(n);
  QDir d(n);
  if((d.exists()) || (d.exists(n)))
    url = QUrl::fromLocalFile(n);
  //else url = QUrl(n); //maybe not locally

  const QFileInfo fileInfo(url.toLocalFile());
  if(!fileInfo.suffix().compare("m3u", Qt::CaseInsensitive))//m3u -> playlist file
      playlist->load(url);

  else if (!fileInfo.suffix().compare("", Qt::CaseInsensitive)) //directory
  {
      QStringList music = d.entryList(QStringList() << "*.wav" << "*.mp3" << "*.flac" << "*.mp4" << "*.m3u", QDir::Files);
      foreach(QString filename, music) playlist->addMedia(QUrl::fromLocalFile(d.path()+ "/" +filename));
  }


  //int b = playlist->mediaCount();

  //configurar playlist
  mediaplayer->setPlaylist(playlist);
  playlist->setPlaybackMode(QMediaPlaylist::PlaybackMode::Loop);
  playlist->shuffle();
  playlist->setCurrentIndex(0);

  mediaplayer->setVolume(100);
}

/*  load task.g file */
void Player::load_file()
{   QString s;
    if(task_iterator >= task.size()) task_iterator %= task.size();
    if(task[task_iterator].sessions <= number) task_iterator++; //start at 0

    /*QTime a = QTime::fromString(task[p].hour1, "h:m");
      QTime b = QTime::fromString(task[p].hour2, "h:m");

      if((!set_day_stage(a, b)) && (!fast_forward))
        {it = it+1%task.size();
         p = it;
        }*/

    s = task[task_iterator].name;
    ui->Phrase->setText(s);
    ui->Phrase->setAlignment(Qt::AlignCenter);
    if (task[task_iterator].playlist != "") load_playlist(task_iterator);
    if (task[task_iterator].script != "") load_scripts(task_iterator);

    end = false;
    mediaplayer->play();
}

/* execute scripts from configuration file */
void Player::load_scripts(int pos)
{
    QString n;
    if (pos > -1 ) n = task[pos].script;
    else n = break_script;

    n.replace(" ", "");

    if (QFile::exists(n))
    { script_process = new QProcess();
      script_process->start(n);
      first_time_pulsed = false;
    }
}
