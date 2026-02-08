#include "BulletManager.h"

#include <QRandomGenerator>

BulletManager::BulletManager(QObject *parent)
    : QAbstractListModel(parent)
{
    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, &QTimer::timeout, this, &BulletManager::onUpdateTick);
    m_updateTimer->start(16);
}

int BulletManager::rowCount(const QModelIndex &parent) const
{
    return m_bulletLists.size();
}

QVariant BulletManager::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_bulletLists.count())
        return QVariant();

    if (role == ObjectRole)
        return QVariant::fromValue(m_bulletLists[index.row()]);

    return QVariant();
}

QHash<int, QByteArray> BulletManager::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ObjectRole] = "bulletObject";
    return roles;
}

const QList<Bullet *> &BulletManager::getBulletsList() const
{
    return m_bulletLists;
}

void BulletManager::onUpdateTick()
{
    if (m_bulletLists.isEmpty())
        return;

    for (int bulletIdx = m_bulletLists.size() - 1; bulletIdx >= 0; --bulletIdx) {
        Bullet *bullet = m_bulletLists[bulletIdx];

        float newY = bullet->bulletY() - m_bulletFallSpeed;
        bullet->setBulletY(newY);

        // TODO : REPLACE MAGIC NUMBERS
        if (newY < -50) {
            removeBullet(bulletIdx);
        }
    }
}

void BulletManager::clearBullets()
{
    beginResetModel();

    qDeleteAll(m_bulletLists);

    m_bulletLists.clear();

    endResetModel();
}

void BulletManager::togglePause(bool paused)
{
    if (paused) {
        m_updateTimer->stop();
    } else {
        m_updateTimer->start(16);
    }
}

void BulletManager::spawnBullet(const int startX,
                                const int startY,
                                const int width,
                                const int height)
{
    const int modelIndex = m_bulletLists.size();

    beginInsertRows(QModelIndex(), modelIndex, modelIndex);

    Bullet *newBullet = new Bullet(startX, startY, width, height, this);

    m_bulletLists.append(newBullet);

    endInsertRows();
}

void BulletManager::removeBullet(int index)
{
    if (index < 0 || index >= m_bulletLists.count())
        return;

    beginRemoveRows(QModelIndex(), index, index);
    Bullet *bullet = m_bulletLists.takeAt(index);
    bullet->deleteLater();
    endRemoveRows();
}
