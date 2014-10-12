#include "result.h"

Result::Result()
{
}

Result::Result(WId winId, const QString& resultTitle, QPixmap icon)
    : winId(winId), resultTitle(resultTitle), icon(icon)
{
}
