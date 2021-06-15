/*
Techniques for enforcing rate limits
In general, a rate is a simple count of occurrences over time. However, there are several different
techniques for measuring and limiting rates, each with their own uses and implications.

Token bucket: A token bucket maintains a rolling and accumulating budget of usage as a balance of tokens.
This technique recognizes that not all inputs to a service correspond 1:1 with requests. A token bucket adds
tokens at some rate. When a service request is made, the service attempts to withdraw a token
(decrementing the token count) to fulfill the request. If there are no tokens in the bucket, 
the service has reached its limit and responds with backpressure. For example, in a GraphQL service,
a single request might result in multiple operations that are composed into a result. These operations
may each take one token. This way, the service can keep track of the capacity that it needs to limit the
use of, rather than tie the rate-limiting technique directly to requests.

Leaky bucket: A leaky bucket is similar to a token bucket, but the rate is limited by the amount that can
drip or leak out of the bucket. This technique recognizes that the system has some degree of finite capacity
to hold a request until the service can act on it; any extra simply spills over the edge and is discarded.
This notion of buffer capacity (but not necessarily the use of leaky buckets) also applies to components
adjacent to your service, such as load balancers and disk I/O buffers.

Fixed window: Fixed-window limits—such as 3,000 requests per hour or 10 requests per day—are easy to state,
but they are subject to spikes at the edges of the window, as available quota resets. Consider, for example,
a limit of 3,000 requests per hour, which still allows for a spike of all 3,000 requests to be made in the
first minute of the hour, which might overwhelm the service.

Sliding window: Sliding windows have the benefits of a fixed window, but the rolling window of time smooths
out bursts. Systems such as Redis facilitate this technique with expiring keys.
*/

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
