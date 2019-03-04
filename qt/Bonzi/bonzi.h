#ifndef BONZI_H
#define BONZI_H

#include <QObject>
#include <QList>
#include <QMap>
#include <QPixmap>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMap>
#include <QDir>
#include <QComboBox>
#include <QPainter>
#include <unistd.h>
#include <QApplication>
#include <QTimer>

typedef struct BranchOption
{
    int location;
    int probability;
} BranchOption;

typedef struct Mouth
{
    int image;
    QString type;
} Mouth;

typedef struct Frame
{
    int duration;
    int exit;
    QString effect;

    QList<BranchOption> branches;

    QList<int> images;
    QList<Mouth> mouth;
} Frame;

typedef struct Animation
{
    QString returnAnimation;
    int transitionType;
    QList<Frame> frames;
} Animation;

class Bonzi
{
    public:
    Bonzi();
    ~Bonzi();

    int loadAnimations(QString filename);
    int setAnimation(QString newAnimation);
    Animation getCurrentAnimation();
    QMap<QString, Animation> getAllAnimations();

    bool isPlaying();
    bool isExiting();
    bool isSpeaking();

    void setPlaying(bool newPlay);
    void setExiting(bool newExit);
    void setSpeaking(bool newSpeak);

    int advanceAnimation();

    private:
    QMap<QString, Animation> animations;
    QMap<int, QImage> images;

    Animation currentAnimation;
    bool playing, exiting, speaking, shouldSpeak;
    int currentFrame, lastImage;
};

#endif // BONZI_H
