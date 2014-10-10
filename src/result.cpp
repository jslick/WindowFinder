#include "result.h"

Result::Result()
{
}

Result::Result(WId winId, const QString& resultTitle)
    : winId(winId), resultTitle(resultTitle)
{
}
