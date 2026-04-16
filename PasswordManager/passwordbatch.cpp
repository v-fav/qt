#include "passwordbatch.h"

QVector<BatchResult> checkAllPasswords(const QVector<BatchItem> &items, std::function<void(int,int)> progress)
{
    QVector<BatchResult> results;

    int current = 0;
    int total = items.size();

    for (const auto &item : items)
    {
        BatchResult r;
        r.row = item.row;
        r.result = checkPasswordSync(item.password);
        results.append(r);

        current++;
        if (progress)
            progress(current, total);
    }

    return results;
}
