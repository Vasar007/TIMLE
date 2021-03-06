﻿#pragma once

#include <cassert>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>


/**
 * \brief             Special class for keep-tracking action with app resources.
 * \tparam Resource   A type of the resources for keeping.
 * \tparam Identifier Array of identificators for tracking recources.
 */
template <class Resource, class Identifier>
class ResourceHolder
{
    private:
        /**
         * \brief Resource map that contains all resources with current type (Identifier).
         */
        std::map<Identifier, std::unique_ptr<Resource>> _resourceMap;


    public:
        /**
         * \brief               Insert new resource (moving pointer to this resource map) and check
         *                      success.
         * \param[in] id        Type identifier of current resources.
         * \param[out] resource Smart pointer that contains type of new resource. Function move
         *                      resource into own container.
         */
        void           insertResource(const Identifier id, std::unique_ptr<Resource> resource);

        /**
         * \brief            Load resource from file.
         * \param id        Type identifier of current resources.
         * \param filename    File name (could be contain file path).
         */
        void            load(const Identifier id, const std::string& filename);

        /**
         * \brief                 Load resource from file with additional parameter.
         * \tparam Parameter      Type of additional parameter (for instance, const IntRect &).
         * \param[in] id          Type identifier of current resources.
         * \param[in] filename    File name (could be contain file path).
         * \param[in] secondParam Additional parameter for loading file (for instance, area of the
         *                        image to load).
         */
        template <class Parameter>
        void            load(const Identifier id, const std::string& filename,
                             const Parameter& secondParam);

        /**
         * \brief        Get resource on the current id.
         * \param[in] id Type identifier of current resources.
         * \return       Reference on the resource in the resource map.
         */
        Resource&       get(const Identifier id);

        /**
         * \brief        Get const resource on the current id.
         * \param[in] id Type identifier of current resources.
         * \return       Const reference on the resource in the resource map.
         */
        const Resource& get(const Identifier id) const;
};

#include "Mechanics/resource_holder.inl"
