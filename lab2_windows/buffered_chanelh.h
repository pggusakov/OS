#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

template<class T>
class BufferedChannel {
public:
	explicit BufferedChannel(int size) : size(size) {}

	void Send(T value) {
		std::unique_lock<std::mutex> lock(g_mutex);

		if (!g_done) {
			throw std::runtime_error("Channel closed!");
		}

		g_queuechecker.wait(lock, [this] {
			return !(g_codes.size() == size);
			}
		);

		g_codes.push(value);
		lock.unlock();
		g_queuechecker.notify_all();
	}

	std::pair<T, bool> Recv() {
		std::unique_lock<std::mutex> lock(g_mutex);
		g_queuechecker.wait(lock, [this] {
			return !is_empty();
			}
		);
		T value = g_codes.front();

		g_codes.pop();
		lock.unlock();
		g_queuechecker.notify_all();
		return std::make_pair(value, g_done);
	}

	void Close() {
		std::unique_ptr<std::mutex> lock(g_mutex);
		g_done = false;
		lock.unlock();
		g_queuechecker.notify_all();
	}

private:

	bool is_empty() { return g_codes.empty(); }

	std::mutex g_mutex;
	bool g_done = true;
	int size;
	std::condition_variable g_queuechecker;
	std::queue<T> g_codes;
};
