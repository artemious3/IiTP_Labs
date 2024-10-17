#include "weirdstar.h"
#include "mymath.h"

WeirdStar::WeirdStar(QObject *parent)
    : AP::Polygon{parent}
{}


WeirdStar::WeirdStar(qreal x, qreal y, int N, qreal length) : AP::Polygon(){


    anchor = {0,0};
    coordinates = {x,y};

    points.reserve(2*N);
    points.shrink_to_fit();

    qreal alpha = 2.0* M_PI / N;
    QVector2D current(length, 0);
    QVector2D one(length*0.2 ,0);
    for(int i = 0; i < N; ++i){
        qreal koef = (qreal)(rand() % 200 - 100) / 100;
        points << (current + one * koef).toPointF();
        current = rotateVector2D(current, alpha);
        one = rotateVector2D(one,alpha);
    }
}
