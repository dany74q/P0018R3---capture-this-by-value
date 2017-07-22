#include <future>
#include <cassert>

namespace P0018R3 {
	class Gadget {
	private:
		int value;
	public:
		Gadget() : value(42) {}

		auto capturesByReferenceAndFails() {
			return std::async(
				[=]() { return value; }
			);
		}

		auto capturesByValueAndSucceeds() {
			return std::async(
				[=, *this]{ return value; }
			);
		}
	};

	auto capturesByReferenceAndFails()
	{
		// The closure associated with the returned future
		// has an implicit this pointer that is invalid.
		return Gadget().capturesByReferenceAndFails();
	}

	auto capturesByValueAndSucceeds() {
		return Gadget().capturesByValueAndSucceeds();
	}
}


int main()
{
	// The following succeeds due to *this being copied by value
	auto f = P0018R3::capturesByValueAndSucceeds();
	f.wait();
	assert(42 == f.get());

	// The following fails due to the
	// originating class having been destroyed
	f = P0018R3::capturesByReferenceAndFails();
	f.wait();
	assert(42 == f.get());	
}