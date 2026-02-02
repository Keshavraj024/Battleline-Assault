#ifndef ENEMY_H
#define ENEMY_H

#include <QObject>
#include <QTimer>
#include "SettingsManager.h"

class Enemy : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double enemyX READ enemyX WRITE setEnemyX NOTIFY enemyXChanged FINAL)
    Q_PROPERTY(double enemyY READ enemyY WRITE setEnemyY NOTIFY enemyYChanged FINAL)
    Q_PROPERTY(int enemyWidth READ enemyWidth WRITE setEnemyWidth NOTIFY enemyWidthChanged FINAL)
    Q_PROPERTY(int enemyHeight READ enemyHeight WRITE setEnemyHeight NOTIFY enemyHeightChanged FINAL)
    Q_PROPERTY(int enemyImageIndex READ enemyImageIndex WRITE setEnemyImageIndex NOTIFY enemyImageIndexChanged FINAL)

public:
    explicit Enemy(QObject *parent = nullptr);

    void initialize();

    double enemyX() const;
    void setEnemyX(double newEnemyX);

    double enemyY() const;
    void setEnemyY(double newEnemyY);

    int enemyWidth() const;
    void setEnemyWidth(int newEnemyWidth);

    int enemyHeight() const;
    void setEnemyHeight(int newEnemyHeight);

    int enemyImageIndex() const;
    void setEnemyImageIndex(int newEnemyImageIndex);

signals:
    void enemyXChanged();
    void enemyYChanged();

    void enemyWidthChanged();

    void enemyHeightChanged();

    void enemyImageIndexChanged();

private slots:
    void updateEnemyPos();

private:
    double m_enemyX;
    double m_enemyY;
    int m_enemyWidth;
    int m_enemyHeight;
    int m_windowHeight;
    int m_windowWidth;

    double m_enemyFallSpeed {5};

    SettingsManager &m_gameControllerSettings = SettingsManager::instance();

    QTimer m_enemyFalltimer;
    int m_enemyImageIndex;
    QList<Enemy *> m_enemyList{};

private:
    friend class GameController;

    void destroyEnemy(Enemy *enemyToDestroy);
    QList<Enemy *> &getEnemyLists();
    void addEnemy();
    bool IsEmpty() const;
    void clearEnemyLists();
    size_t enemyListsSize() const;
    void enemyReset();
    void stopEnemyFallTimer();
    void resumeEnemyFallTimer();
};

#endif // ENEMY_H
