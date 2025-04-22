#include "mainwindow.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QFileInfo>
#include <QDialog>
#include <QListWidget>
#include <QMessageBox>
#include <QStatusBar>

using namespace std;

//Construction of MusicPlayer class, initialize currentsong so no song playing
MusicPlayer:: MusicPlayer(QWidget *parent): QMainWindow(parent), currentsong(-1) {
player = new QMediaPlayer(this); // Object of QMediaPlayer
audioOutput = new QAudioOutput(this);// Object of QAudioOutput
player->setAudioOutput(audioOutput); // Send audio to the speaker
createControls();
}

void MusicPlayer::createControls() {
QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(central);

// Add 5 push buttons
loadButton = new QPushButton("Load Music");
playButton = new QPushButton("Play");
pauseButton = new QPushButton("Pause");
stopButton = new QPushButton("Stop");
playlistButton = new QPushButton("Song Playlist");

// Add buttons to screen layout
layout->addWidget(loadButton);
layout->addWidget(playButton);
layout->addWidget(pauseButton);
layout->addWidget(stopButton);
layout->addWidget(playlistButton);

// Set window size and tittle
setCentralWidget(central);
setWindowTitle("Music Player"); resize(300, 200);

// Link buttons to methods to load, play, pause, stop, and show playlist
connect(loadButton, &QPushButton::clicked, this, &MusicPlayer::loadSong);
connect(playButton, &QPushButton::clicked, this, &MusicPlayer::play);
connect(pauseButton, &QPushButton::clicked, this, &MusicPlayer::pause);
connect(stopButton, &QPushButton::clicked, this, &MusicPlayer::stop);
connect(playlistButton, &QPushButton::clicked, this, &MusicPlayer::showPlaylist);
}
void MusicPlayer::loadSong() {
QString file = QFileDialog::getOpenFileName(this, "Select Song", "", "*.mp3 *.m4a *.mp4"); // Window to load song
if (!file.isEmpty()) { // Check if file path is not empty
    QUrl url = QUrl::fromLocalFile(file);
QFileInfo info(file);

// Check for duplicates
bool alreadyExists = false;
for (const QUrl& existingUrl : playlist) {
if (existingUrl == url) { alreadyExists = true;
    break;
} }

if (!alreadyExists) { playlist.push_back(url);
    songname.push_back(info.fileName());

// Create a temporary vector to hold name-url pairs
    std::vector<QString> temp_songname = songname;
std::vector<QUrl> temp_urls = playlist;

// Simple bubble sort
for (unsigned i = 0; i < temp_songname.size() - 1; ++i) {
for (unsigned j = i + 1; j < temp_songname.size(); ++j) {
if (temp_songname[j].toLower() < temp_songname[i].toLower()) {

// Swap names
QString tempname = temp_songname[i];
temp_songname[i] = temp_songname[j];
temp_songname[j] = tempname;

// Swap corresponding URLs
QUrl tempurl = temp_urls[i];
temp_urls[i] = temp_urls[j];
temp_urls[j] = tempurl;
} }
}

// Replace the original lists with sorted versions
songname = temp_songname;
playlist = temp_urls;
// Set the index to the newly added song
for (unsigned i = 0; i < songname.size(); ++i) {
if (songname[i] == info.fileName()) { currentsong = i;

break; }
} }

player->setSource(url); // load the file locate at url
currentsong = - 1; // Update to the latest song
print("Loaded: " + info.fileName());
} }

void MusicPlayer::play() { // Method to play music
    try {
if (currentsong >= 0 && currentsong < static_cast<int>(playlist.size())) { player->play();
print("Playing: " + songname[currentsong]);
}else{
throw "101";
}
} catch (const char* msg) {
print("Error: " + QString(msg) + "- Please select a song from playlist."); }
}
void MusicPlayer::pause() { // Pause song
    player->pause();
print("Paused.");
}
void MusicPlayer::stop() { // Stop song
    player->stop();
currentsong = -1;
print("Stopped.");
}

void MusicPlayer::showPlaylist() {
QDialog dialog(this); // Pop-up window
    dialog.setWindowTitle("Playlist"); // Title of pop-up window
dialog.resize(400, 300); // Pop-up window size
QVBoxLayout *layout = new QVBoxLayout(&dialog); QListWidget *list = new QListWidget(&dialog);
for (int i = 0; i < static_cast<int>(songname.size()); ++i) { list->addItem(songname[i]);
}
QPushButton *playButton = new QPushButton("Play Selected", &dialog); layout->addWidget(list);
layout->addWidget(playButton);
connect(playButton, &QPushButton::clicked, [&]() {
int row = list->currentRow();
if (row >= 0 && row < static_cast<int>(playlist.size())) {
currentsong = row; player->setSource(playlist[row]); player->play();
print("Playing: " + songname[row]); dialog.accept();
} });
QPushButton *deleteButton = new QPushButton("Delete Selected", &dialog); layout->addWidget(deleteButton);
connect(deleteButton, &QPushButton::clicked, [&]() { int row = list->currentRow();

if (row >= 0 && row < static_cast<int>(playlist.size())) { playlist.erase(playlist.begin() + row); songname.erase(songname.begin() + row);
// Adjust current index if needed
    if (row == currentsong) {
player->stop();
currentsong = -1;
} else if (row < currentsong) {
currentsong--; }
// Refresh the dialog (close and reopen)
dialog.accept();
showPlaylist(); // re-show updated playlist
} });
dialog.exec(); }
void MusicPlayer::print(const QString &message) { statusBar()->showMessage(message);
}
