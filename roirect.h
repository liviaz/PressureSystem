#ifndef ROIRECT_H
#define ROIRECT_H

#include <QObject>
#include <QWidget>
#include <QGraphicsItem>

#define RESIZE_BOTTOM_RIGHT 1
#define RESIZE_BOTTOM_LEFT 2
#define RESIZE_TOP_RIGHT 3
#define RESIZE_TOP_LEFT 4
#define RESIZE_BOTTOM 5
#define RESIZE_RIGHT 6
#define RESIZE_LEFT 7
#define RESIZE_TOP 8
#define HOVER_MIDDLE 9

#define STATE_DRAG 1
#define STATE_RESIZE 2

class RoiRect : public QGraphicsItem
{

public:

    RoiRect(int xTopLeft, int yTopLeft, int width, int height);

    ~RoiRect();

    // defining parameters
    int xTopLeft;
    int yTopLeft;
    int xBottomRight;
    int yBottomRight;
    int width;
    int height;

    // mouse parameters
    int cursorX;
    int cursorY;
    int dragState;
    int hoverState;

    void changeSize(int xTopLeft, int yTopLeft, int width, int height);
    int getCursorLocation(int xEnter, int yEnter);

    QRectF boundingRect() const;

    // overriding paint
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);


protected:

    // overriding mouse events
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);



};

#endif // ROIRECT_H
