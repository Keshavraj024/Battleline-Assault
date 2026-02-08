#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>

class Player : public QObject
{
    Q_OBJECT

    Q_PROPERTY(double playerCurrentX READ playerCurrentX WRITE setPlayerCurrentX NOTIFY
                   playerCurrentXChanged FINAL)
    Q_PROPERTY(double playerCurrentY READ playerCurrentY WRITE setPlayerCurrentY NOTIFY
                   playerCurrentYChanged FINAL)

    Q_PROPERTY(int playerWidth READ playerWidth WRITE setPlayerWidth NOTIFY playerWidthChanged FINAL)
    Q_PROPERTY(
        int playerHeight READ playerHeight WRITE setPlayerHeight NOTIFY playerHeightChanged FINAL)

public:
    explicit Player(double startX, double startY, int width, int height, QObject *parent = nullptr);

    double playerCurrentX() const;
    void setPlayerCurrentX(double newPlayerCurrentX);

    double playerCurrentY() const;
    void setPlayerCurrentY(double newPlayerCurrentY);

    int playerWidth() const;
    void setPlayerWidth(int newPlayerWidth);

    int playerHeight() const;
    void setPlayerHeight(int newPlayerHeight);

signals:
    void playerCurrentXChanged();
    void playerCurrentYChanged();

    void playerWidthChanged();

    void playerHeightChanged();

private:
    double m_playerCurrentX;
    double m_playerCurrentY;

    int m_playerWidth;
    int m_playerHeight;
};

#endif // PLAYER_H
