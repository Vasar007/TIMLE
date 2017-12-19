#ifndef RESOURCEHOLDER_HPP
#define RESOURCEHOLDER_HPP

#include <map>
#include <string>
#include <memory>
#include <stdexcept>
#include <cassert>


/**
 * \brief				Special class for keep-tracking action with app resources.
 * \tparam Resource		A type of the resources for keeping.
 * \tparam Identifier	Array of identificators for tracking recources.
 */
template <typename Resource, typename Identifier>
class ResourceHolder
{
	private:
		/**
		 * \brief Resource map that contains all resources with current type (Identifier).
		 */
		std::map<Identifier, std::unique_ptr<Resource>>	_resourceMap;


	private:
		/**
		 * \brief			Insert new resource (moving pointer to this resource map) and check success.
		 * \param id		Type identifier of current resources.
		 * \param resource	Smart pointer that contains type of new resource.
		 */
		void						insertResource(Identifier id, std::unique_ptr<Resource> resource);


	public:
		/**
		 * \brief			Load resource from file.
		 * \param id		Type identifier of current resources.
		 * \param filename	File name (could be contain file path).
		 */
		void						load(Identifier id, const std::string& filename);

		/**
		 * \brief				Load resource from file with additional parameter.
		 * \tparam Parameter	Type of additional parameter (for instance, const IntRect &).
		 * \param id			Type identifier of current resources.
		 * \param filename		File name (could be contain file path).
		 * \param secondParam	Additional parameter for loading file (for instance, area of the image to load).
		 */
		template <typename Parameter>
		void						load(Identifier id, const std::string& filename, const Parameter& secondParam);

		/**
		 * \brief		Get resource on the current id.
		 * \param id	Type identifier of current resources.
		 * \return		Reference on the resource in the resource map.
		 */
		Resource&					get(Identifier id);

		/**
		 * \brief		Get const resource on the current id.
		 * \param id	Type identifier of current resources.
		 * \return		Const reference on the resource in the resource map.
		 */
		const Resource&				get(Identifier id) const;
};

#include "ResourceHolder.inl"

#endif // RESOURCEHOLDER_HPP
