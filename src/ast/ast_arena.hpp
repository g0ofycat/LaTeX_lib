#ifndef AST_ARENA_HPP
#define AST_ARENA_HPP

#include <vector>
#include <cstddef>
#include <memory>
#include <utility>

// ======================
// -- ASTArena
// ======================

class ASTArena
{
public:
    std::vector<std::byte *> chunks;
    size_t offset = 0;
    static constexpr size_t CHUNK_SIZE = 65536;

    struct ManagedObject
    {
        void *ptr;
        void (*destructor)(void *);
    };

    std::vector<ManagedObject> managed_objects;

    /// @brief Allocate a new chunk based on CHUNK_SIZE
    void allocate_new_chunk()
    {
        chunks.push_back(new std::byte[CHUNK_SIZE]);
        offset = 0;
    }

public:
    /// @brief Custom std::unique_ptr to avoid overhead
    /// @tparam T Template
    /// @tparam ...Args Template
    /// @param ...args The data to allocate
    /// @return Memory Address of allocated data
    template <typename T, typename... Args>
    T *alloc(Args &&...args)
    {
        constexpr size_t size = sizeof(T);
        constexpr size_t alignment = alignof(T);

        if (chunks.empty() || offset + size + alignment > CHUNK_SIZE)
            allocate_new_chunk();

        std::byte *base = chunks.back();
        std::byte *aligned = base + ((offset + alignment - 1) & ~(alignment - 1));

        if (aligned + size > base + CHUNK_SIZE)
        {
            allocate_new_chunk();
            base = chunks.back();
            aligned = base;
        }

        offset = (aligned - base) + size;

        T *result = new (aligned) T(std::forward<Args>(args)...);

        if constexpr (!std::is_trivially_destructible_v<T>)
        {
            managed_objects.push_back({result, [](void *p)
                                       { static_cast<T *>(p)->~T(); }});
        }

        return result;
    }

    // ======================
    // -- CONSTRUCTOR
    // ======================

    /// @brief Default constructor
    ASTArena() = default;

    // ======================
    // -- DESTRUCTOR
    // ======================

    /// @brief Destructor for ASTArena
    ~ASTArena()
    {
        for (auto it = managed_objects.rbegin(); it != managed_objects.rend(); ++it)
            it->destructor(it->ptr);

        for (auto *chunk : chunks)
            delete[] chunk;
    }

    // ======================
    // -- MISC
    // ======================

    /// @brief Prevent copies
    ASTArena(const ASTArena &) = delete;

    /// @brief Prevent copies
    /// @return ASTArena
    ASTArena &operator=(const ASTArena &) = delete;
};

#endif