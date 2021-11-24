#include "../../libs/uncompressed_structures/edgeLog.h"

EdgeLog::EdgeLog() {}

EdgeLog::~EdgeLog() {}

void EdgeLog::addEdge(uint u, uint v, uint t1, uint t2) {
    const vector<uint> vec = {u, v, t1, t2};
    log.push_back(vec);
}