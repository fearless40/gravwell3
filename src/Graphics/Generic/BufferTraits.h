
#pragma once
#include <type_traits>

namespace Graphics::Generic::BufferTraits {
	namespace Binding {
		struct Default {
			using is_binding = std::true_type;
		};

		struct Dynamic {
			using is_binding = std::true_type;
		};

		struct Immutable {
			using is_binding = std::true_type;
		};

		struct Staging {
			using is_binding = std::true_type;
		};
	};

	namespace CPUAccess {
		struct Default {
			using is_cpuaccess = std::true_type;
		};
		struct Read {
			using is_cpuaccess = std::true_type;
		};
		struct Write { 
			using is_cpuaccess = std::true_type; 
		};
		struct ReadWrite { 
			using is_cpuaccess = std::true_type; 
		};
	};

	struct Default {
		using Binding = Binding::Default;
		using CPU = CPUAccess::Default;
	};

	struct Dynamic {
		using Binding = Binding::Dynamic;
		using CPU = CPUAccess::Write;
	};

	struct Immutable {
		using Binding = Binding::Immutable;
		using CPU = CPUAccess::Default;
	};
}