#pragma once

template <class Resource, class Identifier>
void ResourceHolder<Resource, Identifier>::load(const Identifier id, const std::string& filename)
{
    // Create and load resource.
    auto resource = std::make_unique<Resource>();
    if (!resource->loadFromFile(filename))
    {
        throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);
    }

    // If loading successful, insert resource to map.
    insertResource(id, std::move(resource));
}

template <class Resource, class Identifier>
template <class Parameter>
void ResourceHolder<Resource, Identifier>::load(const Identifier id, const std::string& filename,
                                                const Parameter& secondParam)
{
    // Create and load resource.
    auto resource = std::make_unique<Resource>();
    if (!resource->loadFromFile(filename, secondParam))
    {
        throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);
    }

    // If loading successful, insert resource to map.
    insertResource(id, std::move(resource));
}

template <class Resource, class Identifier>
Resource& ResourceHolder<Resource, Identifier>::get(const Identifier id)
{
    auto found = _resourceMap.find(id);
    assert(found != _resourceMap.end());

    return *found->second;
}

template <class Resource, class Identifier>
const Resource& ResourceHolder<Resource, Identifier>::get(const Identifier id) const
{
    auto found = _resourceMap.find(id);
    assert(found != _resourceMap.end());

    return *found->second;
}

template <class Resource, class Identifier>
void ResourceHolder<Resource, Identifier>::insertResource(const Identifier id,
                                                          std::unique_ptr<Resource> resource) 
{
    // Insert and check success.
    [[maybe_unused]] auto inserted = _resourceMap.emplace(id, std::move(resource));
    assert(inserted.second);
}
