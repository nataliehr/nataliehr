#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QListWidget>
#include <QUrl>
#include <vector>
using namespace std;

class QPushButton;
class MusicPlayer : public QMainWindow {
    Q_OBJECT
public:
    explicit MusicPlayer(QWidget *parent = nullptr);
private:
    QMediaPlayer *player;
    QAudioOutput *audioOutput;
    QPushButton *loadButton;
    QPushButton *playButton;
    QPushButton *pauseButton;
    QPushButton *stopButton;
    QPushButton *playlistButton;
    vector<QUrl> playlist;
    vector<QString> songname;
    int currentsong;
    void createControls();
    void loadSong();
    void play();
    void pause();
    void stop();
    void showPlaylist();
    void print(const QString &message);
};
#endif // MAINWINDOW_H
