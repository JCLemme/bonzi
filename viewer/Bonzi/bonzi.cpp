#include "bonzi.h"

Bonzi::Bonzi()
{

}

Bonzi::~Bonzi()
{

}

int Bonzi::loadAnimations(QString filename)
{
    // I get paid every time I add an "anim"
    QFile animFile(filename);
    animFile.open(QIODevice::ReadOnly);
    QByteArray animData = animFile.readAll();

    QJsonDocument animDoc(QJsonDocument::fromJson(animData));
    QJsonObject animRoot = animDoc.object();

    // Get the array of animations
    QJsonArray animArray = animRoot["animations"].toArray();

    for(auto && element: animArray)
    {
        Animation animation;
        QString name = element.toObject()["name"].toString();
        animation.returnAnimation = element.toObject()["return"].toString();
        animation.transitionType = element.toObject()["transitionType"].toInt();

        // Get the array of frames
        QJsonArray animFrames = element.toObject()["frames"].toArray();

        for(auto && felement: animFrames)
        {
            Frame frame;

            frame.duration = felement.toObject()["duration"].toInt();
            frame.exit = felement.toObject()["exit"].toInt() - 1;  // ACX uses 1 as the starting frame index, so subtract 1 here to make the index 0
            frame.effect = felement.toObject()["effect"].toString();
            QJsonArray frameImages = felement.toObject()["images"].toArray();
            QJsonArray frameMouth = felement.toObject()["mouth"].toArray();
            QJsonArray frameBranch = felement.toObject()["branches"].toArray();

            for(auto && ielement: frameImages)
            {
                frame.images.append(ielement.toInt());
            }

            for(auto && melement: frameMouth)
            {
                Mouth mouth;
                mouth.image = melement.toInt();
                mouth.type = melement.toString();
                frame.mouth.append(mouth);
            }

            for(auto && belement: frameBranch)
            {
                BranchOption branch;
                branch.location = belement.toInt() - 1;  // ACX uses 1 as the starting frame index, so subtract 1 here to make the index 0
                branch.probability = belement.toInt();
                frame.branches.append(branch);
            }

            animation.frames.append(frame);
        }

        qDebug() << name;
        ui->animationBox->addItem(name);
        animations[name] = animation;
    }

    // Load dem images
    QDir imageDir("/home/jclemme/Projects/bonzi/bonzi_data/Images/");
    QStringList imageList = imageDir.entryList(QStringList() << "*.png" << "*.PNG", QDir::Files);

    foreach(QString filename, imageList)
    {
        QImage input = QImage("/home/jclemme/Projects/bonzi/bonzi_data/Images/" + filename);
        int index = filename.split(".")[0].toInt();
        qDebug() << input.isNull();
        images[index] = input.convertToFormat(QImage::Format_ARGB32);
    }

    QPixmap temp = QPixmap::fromImage(images[1]);
    QPainter paint(&temp);
    paint.fillRect(temp.rect(), Qt::transparent);
    paint.drawImage(0, 0, images[0]);
    ui->viewer->setPixmap(temp);
    ui->viewer->show();

    lastImage = 1;

    return 0;
}

int Bonzi::setAnimation(QString newAnimation)
{
    if(!animations.contains(newAnimation))
        return -1;

    currentAnimation = animations[newAnimation];
    return 0;
}

Animation Bonzi::getCurrentAnimation()
{
    return currentAnimation;
}

QMap<QString, Animation> Bonzi::getAllAnimations()
{
    return animations;
}

bool Bonzi::isPlaying()
{
    return playing;
}

bool Bonzi::isExiting()
{
    return exiting;
}

bool Bonzi::isSpeaking()
{
    return speaking;
}

void Bonzi::setPlaying(bool newPlay)
{
    playing = newPlay;
}

void Bonzi::setExiting(bool newExit)
{
    exiting = newExit;
}

void Bonzi::setSpeaking(bool newSpeak)
{
    shouldSpeak = newSpeak;
}

int Bonzi::advanceAnimation()
{
    // Show the current frame
    QPixmap img = QPixmap::fromImage(images[lastImage]);
    QPainter painter1(&img);

    // Clear screen
    if(currentAnimation.frames[currentFrame].duration != 0 && currentAnimation.frames[currentFrame].images.size() != 0)
    {
        painter1.setCompositionMode(QPainter::CompositionMode_Source);
        painter1.fillRect(img.rect(), Qt::transparent);
        painter1.setCompositionMode(QPainter::CompositionMode_SourceOver);
    }

    // Copy body images
    for(int i=0;i<currentAnimation.frames[currentFrame].images.size();i++)
    {
        painter1.drawImage(0, 0, images[currentAnimation.frames[currentFrame].images[i]]);
        lastImage = currentAnimation.frames[currentFrame].images[i];
    }

    // Copy mouth image
    for(int i=0;i<currentAnimation.frames[currentFrame].images.size();i++)
    {
        painter1.drawImage(0, 0, images[currentAnimation.frames[currentFrame].images[i]]);
    }

    ui->viewer->setPixmap(img);
    ui->viewer->show();

    // Cache the current frame's delay
    int delay = currentAnimation.frames[currentFrame].duration;

    // If there's a mouth frame and we aren't exiting and we should be speaking, set the speaking flag
    if(!exiting && shouldSpeak && currentAnimation.frames[currentFrame].mouth.size() > 0)
    {
        speaking = true;
    }
    else
    {
        speaking = false;
    }

    // Alright if we're *not* speaking
    if(!speaking)
    {
        // Figure out what frame we're going to next
        if(!exiting && currentAnimation.frames[currentFrame].branches.size() > 0)
        {
            int moveOn = 100;

            for(int i=0;i<currentAnimation.frames[currentFrame].branches.size();i++)
            {
                painter1.drawImage(0, 0, images[currentAnimation.frames[currentFrame].images[i]]);
                lastImage = currentAnimation.frames[currentFrame].images[i];
            }
        }
        else if(exiting && currentAnimation.transitionType == 1 && currentAnimation.frames[currentFrame].exit >= 0)
        {
            // If the animation is exiting, and the animation uses exit branches, and there is an exit branch, use it
            currentFrame = currentAnimation.frames[currentFrame].exit;
        }
        else
        {
            currentFrame++;

            if(currentFrame >= currentAnimation.frames.size())
            {
                exiting = true;

                if(exiting && currentAnimation.transitionType == 0)
                {
                    currentAnimation = animations[currentAnimation.returnAnimation];
                    currentFrame = 0;
                    exiting = false;
                    playing = true;
                }
                else
                {
                    playing = false;
                }
            }
        }
    }

    if(playing)
        return delay;
    else
        return 0;
}
