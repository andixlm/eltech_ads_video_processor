#include "player.h"

const int FRAME_CHANNEL_RGB = 3;

Player::Player(QObject* parent) :
    QThread(parent)
{
    mIsStopped = true;
}

Player::~Player()
{
    mMutex.lock();

    mIsStopped = true;
    mCapture.release();
    mWaitCondition.wakeOne();

    mMutex.unlock();

    wait();
}

bool Player::isStopped() const
{
    return mIsStopped;
}

int Player::getFrameRate() const
{
    return mFrameRate;
}

void Player::play()
{
    if (!isRunning()) {
        if (isStopped()) {
            mIsStopped = false;
        }

        start(LowPriority);
    }
}

void Player::pause()
{
    mIsStopped = true;
}

bool Player::loadVideo(std::string fileName)
{
    mCapture.open(fileName);

    if (mCapture.isOpened()) {
        mFrameRate =
                static_cast<int>(mCapture.get(CV_CAP_PROP_FPS));
        return true;
    }

    return false;
}

bool Player::closeVideo()
{
    if (!mCapture.isOpened())
        return false;

    mCapture.release();

    return true;
}

void Player::run()
{
    int delay = 1000 / mFrameRate;

    while(!mIsStopped) {
        if (!mCapture.read(mFrame))
            mIsStopped = true;

        if (mFrame.channels() == FRAME_CHANNEL_RGB) {
            cv::cvtColor(mFrame, mRGBFrame, CV_BGR2RGB);
            mImage = QImage(static_cast<const unsigned char*>(mRGBFrame.data),
                                  mRGBFrame.cols, mRGBFrame.rows, QImage::Format_RGB888);
        } else {
            mImage = QImage(static_cast<const unsigned char*>(mFrame.data),
                            mFrame.cols, mFrame.rows, QImage::Format_Indexed8);
        }

        emit processedImage(mImage);
        msleep(delay);
    }
}
