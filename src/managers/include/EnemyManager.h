#pragma once

#include <QAbstractListModel>
#include <qqmlintegration.h>

#include <QList>
#include "Enemy.h"

class EnemyManager : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

    enum EnemyRoles { ObjectRole = Qt::UserRole + 1 };

public:
    explicit EnemyManager(QObject *parent = nullptr);

    void spawnEnemy(int windowWidth);
    void removeEnemy(int index);

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    const QList<Enemy *> &getEnemiesList() const;

    void togglePause(bool paused);

    void clearEnemies();

private:
    QList<Enemy *> m_enemyLists;

    QTimer *m_updateTimer;
    float m_enemyFallSpeed{5.f};

    // QAbstractItemModel interface
private slots:
    void onUpdateTick();
};
