#include "roirect.h"
#include <QPainter>
#include <QtGlobal>
#include <QDebug>
#include <QGraphicsSceneHoverEvent>

RoiRect::RoiRect(int xTopLeft, int yTopLeft, int width, int height)
{

    this->xTopLeft = xTopLeft;
    this->yTopLeft = yTopLeft;
    this->xBottomRight = xTopLeft + width;
    this->yBottomRight = yTopLeft + height;
    this->width = width;
    this->height = height;

    // make this rectangle movable
    setFlag(ItemIsMovable);
    dragState = STATE_DRAG;
    this->setAcceptHoverEvents(true);
    this->setAcceptDrops(true);


}


RoiRect::~RoiRect()
{

}

// resize through command
void RoiRect::changeSize(int xTopLeft, int yTopLeft, int width, int height)
{

    this->xTopLeft = xTopLeft;
    this->yTopLeft = yTopLeft;
    this->xBottomRight = xTopLeft + width;
    this->yBottomRight = yTopLeft + height;
    this->width = width;
    this->height = height;

    // make this rectangle movable
    setFlag(ItemIsMovable);
}


QRectF RoiRect::boundingRect() const
{
    return QRectF(xTopLeft, yTopLeft, width, height);
}


void RoiRect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    QRectF rect = QRectF(xTopLeft+5, yTopLeft+5, width-10, height-10);
    QPen pen(Qt::red, 3);

    painter->setPen(pen);
    painter->drawRect(rect);

}


void RoiRect::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

    // on mouse down, decide if we are dragging or resizing
    if (hoverState == HOVER_MIDDLE){
        dragState = STATE_DRAG;
    } else {
        dragState = STATE_RESIZE;
    }

    QGraphicsItem::mousePressEvent(event);
    update();

}


void RoiRect::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

    QGraphicsItem::mouseReleaseEvent(event);
    update();
}


void RoiRect::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

    int xInit = event->lastPos().x();
    int yInit = event->lastPos().y();
    int xFinal = event->pos().x();
    int yFinal = event->pos().y();

    this->prepareGeometryChange();

    if (dragState == STATE_DRAG){

        xTopLeft += xFinal - xInit;
        xBottomRight += xFinal - xInit;
        yTopLeft += yFinal - yInit;
        yBottomRight += yFinal - yInit;

    } else {

        // resize
        switch (hoverState){

        case RESIZE_BOTTOM_RIGHT:
            xBottomRight += xFinal - xInit;
            yBottomRight += yFinal - yInit;
            break;
        case RESIZE_TOP_LEFT:
            xTopLeft += xFinal - xInit;
            yTopLeft += yFinal - yInit;
            break;
        case RESIZE_BOTTOM_LEFT:
            xTopLeft += xFinal - xInit;
            yBottomRight += yFinal - yInit;
            break;
        case RESIZE_TOP_RIGHT:
            xBottomRight += xFinal - xInit;
            yTopLeft += yFinal - yInit;
            break;
        case RESIZE_BOTTOM:
            yBottomRight += yFinal - yInit;
            break;
        case RESIZE_TOP:
            yTopLeft += yFinal - yInit;
            break;
        case RESIZE_LEFT:
            xTopLeft += xFinal - xInit;
            break;
        case RESIZE_RIGHT:
           xBottomRight += xFinal - xInit;
           break;
        }

        width = xBottomRight - xTopLeft;
        height = yBottomRight - yTopLeft;
    }

    update();
}


void RoiRect::hoverEnterEvent(QGraphicsSceneHoverEvent *event){

    update();
    int xEnter = event->scenePos().x();
    int yEnter = event->scenePos().y();

    // check where cursor enters
    hoverState = getCursorLocation(xEnter, yEnter);
//    qDebug() << "cursorEnterLocation: " << hoverState;

    switch (hoverState){

    case RESIZE_BOTTOM_RIGHT:
    case RESIZE_TOP_LEFT:
        this->setCursor(Qt::SizeFDiagCursor);
        break;
    case RESIZE_BOTTOM_LEFT:
    case RESIZE_TOP_RIGHT:
        this->setCursor(Qt::SizeBDiagCursor);
        break;
    case RESIZE_BOTTOM:
    case RESIZE_TOP:
        this->setCursor(Qt::SizeVerCursor);
        break;
    case RESIZE_LEFT:
    case RESIZE_RIGHT:
       this->setCursor(Qt::SizeHorCursor);
       break;
    }

    update();

}


void RoiRect::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    this->setCursor(Qt::ArrowCursor);
}


void RoiRect::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    update();
    int xCurr = event->scenePos().x();
    int yCurr = event->scenePos().y();

    // check where cursor enters
    hoverState = getCursorLocation(xCurr, yCurr);
//    qDebug() << "cursorEnterLocation: " << hoverState;

    switch (hoverState){

    case RESIZE_BOTTOM_RIGHT:
    case RESIZE_TOP_LEFT:
        this->setCursor(Qt::SizeFDiagCursor);
        break;
    case RESIZE_BOTTOM_LEFT:
    case RESIZE_TOP_RIGHT:
        this->setCursor(Qt::SizeBDiagCursor);
        break;
    case RESIZE_BOTTOM:
    case RESIZE_TOP:
        this->setCursor(Qt::SizeVerCursor);
        break;
    case RESIZE_LEFT:
    case RESIZE_RIGHT:
       this->setCursor(Qt::SizeHorCursor);
       break;
    default:
        this->setCursor(Qt::SizeAllCursor);
        break;
    }

    update();
}


// cursor must be within 10 pixels of each corner / side to be considered
// as "over" that location
int RoiRect::getCursorLocation(int xEnter, int yEnter)
{

    int thresh = 20;

//    qDebug() << "mouse enter (x,y): " << xEnter << ", " << yEnter;
//    qDebug() << "rect boundaries: (xLeft, yTop, xRight, yBottom): " <<
//                xTopLeft << "," << yTopLeft << "," << xBottomRight << "," << yBottomRight;

    // check if cursor close to left
    if (qAbs(xEnter - xTopLeft) < thresh){

        if (qAbs(yEnter - yTopLeft) < thresh){
            // check if close to top
            return RESIZE_TOP_LEFT;
        } else if (qAbs(yEnter - yBottomRight) < thresh){
            // check if close to bottom
            return RESIZE_BOTTOM_LEFT;
        } else {
            return RESIZE_LEFT;
        }

    // check if cursor close to right
    } else if (qAbs(xEnter - xBottomRight) < thresh) {

        if (qAbs(yEnter - yTopLeft) < thresh){
            // check if close to top
            return RESIZE_TOP_RIGHT;
        } else if (qAbs(yEnter - yBottomRight) < thresh){
            // check if close to bottom
            return RESIZE_BOTTOM_RIGHT;
        } else {
            return RESIZE_RIGHT;
        }

    // otherwise, check if close to top or bottom
    } else {

        if (qAbs(yEnter - yTopLeft) < thresh){
            // check if close to top
            return RESIZE_TOP;
        } else if (qAbs(yEnter - yBottomRight) < thresh){
            // check if close to bottom
            return RESIZE_BOTTOM;
        } else {
            return HOVER_MIDDLE;
        }
    }
}





