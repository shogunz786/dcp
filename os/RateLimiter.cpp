/* Rate limiter.  Limit calls to doWork to n per interval.  Call doDrop if the 
	limit has been reached for the interval.
	Assume doWork and doDrop are asynchronous functions.
*/
queue<int> timeq;
int intervalMS = 2000; // two seconds (as an example)
int n = 10; // 10 requests per 2 seconds (as an example)

void OnRequestRecieve(Req& r) {
	int time = timeGetTime(); // set time since boot in milliseconds.
	if (timeq.size() < n) { // trivial case.  we are not at the limit.
		timeq.push(time);
		doWork(r);
	} else {
		if ((time - timeq.front()) > intervalMS) { 
			//oldest req interval has expired lets work 
			timeq.pop();
			timeq.push(time);
			doWork(r);
		} else { // no space in timeq, and all requests in timeq are still valid.
			doDrop(r);
		}
	}
}
