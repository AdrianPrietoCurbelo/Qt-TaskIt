#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTime>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QTreeWidgetItem>
#include <QTextStream>
#include <random>
#include <QFileDialog>
#include <QVector>
#include <QList>
#include <QTreeView>
#include <QFile>
#include <QGroupBox>
#include <QSignalMapper>
#include <QPixmap>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QTextStream>
#include <QPixmap>
#include <fstream>
#include <string>
#include <cstdlib>
#include <time.h>
#include <algorithm>
#include <chrono>
#include <QDate>
#include <QProcess>
#include <QAudioFormat>
#include <QtMath>
#include <QtEndian>
#include <QXmlStreamReader>
#include <QMessageBox>
#include <QIcon>

/**
 * @author Adrian Prieto Curbelo
 * @version 1.0.0
 * @date 15 August 2018
 * @email alu0100948387@ull.edu.es / adrianpc@gmail.com
 * @title Task-It!
 */

struct Task
{ QString name;
    //hour1, hour2;
  QString playlist;
  QString script;
  int sessions;
  bool done;  
  unsigned short type;
};

namespace Ui {
class MainWindow;
}

class Player: public QMainWindow
{
    Q_OBJECT

    Ui::MainWindow *ui;
    QTimer* timer;
    QTime fin ;
    QMediaPlayer* mediaplayer;
    QMediaPlaylist* playlist;
    QTime* time;
    bool end, start, pomodoro, first_time_pulsed;
    int break_time = 5, pomodoro_time = 25, ult_break_time = 10, number = 0, task_iterator = 0;
    QProcess* script_process;

    QVector<Task> task;

    QString break_script, break_play, to_do;

    //const QString spotify_play_script_path= "";
    //const QString spotify_pause_script_path= "";

public slots:
    void showTime();
    void pause();
    void play();
    void restart();
    void stop();
    void previous();
    void next();
    void done();
    void switch_play_state();

public:
    Player(QWidget* parent = 0);
    ~Player();
    void load_playlist(int pos = -1);
    void load_scripts(int pos = -1);
    void clock_start();
    void set_break_time(const int &t){break_time = t;}
    void set_ult_break_time(const int &t){ult_break_time = t;}
    void set_pomodoro_time (const int &t) {pomodoro_time = t;}
    void set_break_script (const QString & t){ break_script = t;}
    void set_break_play (const QString & t){ break_play = t;}
    void set_end(const bool & t){end = t;}
    void load_file();
    const int& get_number() {return number;}
    void set_number(const int& t) {number = t;}
    void add_task(const Task& new_task){task.push_back(new_task);}
};

class FileManager
{ QFile* f;

  public:
    void load_auto(QString name, Player* mediaplayer);
};

class MainWindow : public QMainWindow
{
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    //IN LOAD.CPP
    void load_auto(QString name);
    void save_auto(QString name);
    void load_playlist(int pos = -1);
    void load_scripts(int pos = -1);
    void log_save();
    void log_load();

    int set_it();

    void set_counter(QString option);
    void set_playlist(QString option);
    void set_scripts(QString option);
    void set_wallpapers(QString option);

    bool set_day_stage();
    bool set_day_stage(QTime a, QTime b);

    void readTimefunct(QStringList func);
    void readRoutinefunct(QStringList func);
    void load_functions(QTextStream&);

    //IN SLOT.CPP
    void get_checked();
    void clock_start();

    //IN FFT.CPP
    QByteArray PitchShift(long offset, long sampleCount, long osamp, float sampleRate, QVector<float> indata,long fftFrameSize);
    void ShortTimeFourierTransform(QVector<float>& fftBuffer, long fftFrameSize, long sign);
    void SetPitchShiftValue();

    void show_player();
    void read_args(QString);

private:

    QVector <QString> files;

    Player* mediaplayer;
    FileManager* filemanager;

    int day = QDate::currentDate().day();
    int month = QDate::currentDate().month();

  QVector <int> prior, nonprior, list;
        //include pos (100*i+j) of daily and non-daily


    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    bool  // if was on pomodoro or resting
        first_time,
        fast_forward = false;
    int chosen;

    QString path, playlist_path, wallpaper_path, script_path;

    //optional variables
    float pitchShift;
    int trackPitch;
};



#endif
