#ifndef PLAYERBULLET_H
#define PLAYERBULLET_H

#include <QObject>
#include <QTimer>
#include "SettingsManager.h"

class PlayerBullet : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double bulletX READ bulletX WRITE setBulletX NOTIFY bulletXChanged FINAL)
    Q_PROPERTY(double bulletY READ bulletY WRITE setBulletY NOTIFY bulletYChanged FINAL)
    Q_PROPERTY(int bulletWidth READ bulletWidth WRITE setBulletWidth NOTIFY bulletWidthChanged FINAL)
    Q_PROPERTY(int bulletHeight READ bulletHeight WRITE setBulletHeight NOTIFY bulletHeightChanged FINAL)

public:
    explicit PlayerBullet(SettingsManager& settings,QObject *parent = nullptr);

    void initialize(SettingsManager& settings);

    double bulletX() const;
    void setBulletX(double newBulletX);

    double bulletY() const;
    void setBulletY(double newBulletY);

    int bulletWidth() const;
    void setBulletWidth(int newBulletWidth);

    int bulletHeight() const;
    void setBulletHeight(int newBulletHeight);

signals:
    void bulletXChanged();
    void bulletYChanged();

    void bulletWidthChanged();

    void bulletHeightChanged();

    void bulletOutofWindow(PlayerBullet* bulletToDestroy);

private slots:
    void updateBulletPos();

private:
    double m_bulletX;
    double m_bulletY;
    double m_ySpeed {5.0};

    QTimer m_timer;
    int m_bulletWidth;
    int m_bulletHeight;
};

#endif // PLAYERBULLET_H
