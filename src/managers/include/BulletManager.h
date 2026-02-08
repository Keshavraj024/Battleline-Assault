#pragma once

#include <QAbstractListModel>
#include <qqmlintegration.h>

#include <QList>
#include "Bullet.h"

class BulletManager : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

    enum BulletRoles { ObjectRole = Qt::UserRole + 1 };

public:
    explicit BulletManager(QObject *parent = nullptr);

    void spawnBullet(const int playerX, const int playerY, const int width, const int height);
    void removeBullet(int index);

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    const QList<Bullet *> &getBulletsList() const;

    void togglePause(bool paused);

    void clearBullets();

private:
    QList<Bullet *> m_bulletLists;

    QTimer *m_updateTimer;
    float m_bulletFallSpeed{0.8f};

private slots:
    void onUpdateTick();
};
