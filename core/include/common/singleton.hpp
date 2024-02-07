
#include <memory>

namespace Common
{
    template <class T>
    class Singleton
    {
        Singleton(const Singleton &) = delete;
        Singleton(Singleton &&) = delete;

    public:
        inline static std::unique_ptr<T> instance{new T()};
        
    };
}