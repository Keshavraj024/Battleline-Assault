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
    Q_PROPERTY(QString imageSource READ imageSource CONSTANT);

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

    QString imageSource() const;
    void setImageSource(const QString &source);

signals:
    void enemyXChanged();
    void enemyYChanged();

    void enemyWidthChanged();
    void enemyHeightChanged();

private:
    double m_enemyX;
    double m_enemyY;

    int m_enemyWidth;
    int m_enemyHeight;

    int m_windowHeight;
    int m_windowWidth;

    SettingsManager &m_gameControllerSettings{SettingsManager::instance()};

    QString m_imageSource;

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
