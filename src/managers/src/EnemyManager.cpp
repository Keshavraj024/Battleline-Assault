#include "EnemyManager.h"

#include <QRandomGenerator>

EnemyManager::EnemyManager(QObject *parent)
    : QAbstractListModel(parent)
{
    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, &QTimer::timeout, this, &EnemyManager::onUpdateTick);
    m_updateTimer->start(16);
}

int EnemyManager::rowCount(const QModelIndex &parent) const
{
    return m_enemyLists.size();
}

QVariant EnemyManager::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_enemyLists.count())
        return QVariant();

    if (role == ObjectRole)
        return QVariant::fromValue(m_enemyLists[index.row()]);

    return QVariant();
}

QHash<int, QByteArray> EnemyManager::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ObjectRole] = "enemyObject";
    return roles;
}

const QList<Enemy *> &EnemyManager::getEnemiesList() const
{
    return m_enemyLists;
}

void EnemyManager::onUpdateTick()
{
    if (m_enemyLists.isEmpty())
        return;

    for (int enemyIdx = m_enemyLists.size() - 1; enemyIdx >= 0; --enemyIdx) {
        Enemy *enemy = m_enemyLists[enemyIdx];

        float newY = enemy->enemyY() + m_enemyFallSpeed;
        enemy->setEnemyY(newY);

        if (newY > 800) {
            removeEnemy(enemyIdx);
        }
    }
}

void EnemyManager::clearEnemies()
{
    beginResetModel();

    qDeleteAll(m_enemyLists);

    m_enemyLists.clear();

    endResetModel();
}

void EnemyManager::togglePause(bool paused)
{
    if (paused) {
        m_updateTimer->stop();
    } else {
        m_updateTimer->start(16);
    }
}

void EnemyManager::spawnEnemy(const int width, const int height, const int windowWidth)
{
    static const QStringList imagesURL{
        "qrc:/qml/Assets/Enemies/AbyssalHexcrawler.png",
        "qrc:/qml/Assets/Enemies/EmberclawRavager.png",
        "qrc:/qml/Assets/Enemies/IronbaneSkitterlord.png",
        "qrc:/qml/Assets/Enemies/NetherfangSentinel.png",
        "qrc:/qml/Assets/Enemies/PyroniteWarDrone.png",
        "qrc:/qml/Assets/Enemies/VoidreaverStrix.png",
        "qrc:/qml/Assets/Enemies/PyrothorSovereign.png",
        "qrc:/qml/Assets/Enemies/VoidclawIncinerator.png",
        "qrc:/qml/Assets/Enemies/AshmawSpectralite.png",
        "qrc:/qml/Assets/Enemies/BlazefangOverseer.png",
        "qrc:/qml/Assets/Enemies/InfernicTalonspawn.png",
        "qrc:/qml/Assets/Enemies/CindervoidRavager.png",
    };

    int startX = QRandomGenerator::global()->bounded(50, windowWidth - 50);
    const int modelIndex = m_enemyLists.size();

    int randomImageIndex = QRandomGenerator::global()->bounded(imagesURL.size());

    beginInsertRows(QModelIndex(), modelIndex, modelIndex);

    Enemy *newEnemy = new Enemy(width, height, this);
    newEnemy->setEnemyX(startX);
    newEnemy->setEnemyY(-50);
    newEnemy->setImageSource(imagesURL[randomImageIndex]);

    m_enemyLists.append(newEnemy);

    endInsertRows();
}

void EnemyManager::removeEnemy(int index)
{
    if (index < 0 || index >= m_enemyLists.count())
        return;

    beginRemoveRows(QModelIndex(), index, index);
    Enemy *enemy = m_enemyLists.takeAt(index);
    enemy->deleteLater();
    endRemoveRows();
}
