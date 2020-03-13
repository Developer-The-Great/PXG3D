#pragma once
#include <vector>
#include <cstdint> //std::uint16_t
namespace PXG
{
	/**@class subject_base
	 * @brief the subject in the Observer-Pattern
	 * should be used as a base-class to give an object
	 * the ability to notify other classes
	 */
	class subject_base
	{
	public:
		virtual ~subject_base() = default;


		/**@brief your custom events (i.e.: enums)
		 * should inherit from even_t to ensure compatibility
		 * with the onNotify adapter. the values can be chosen arbitrary for each
		 * subject as a const object-ptr is also passed
		 */
		typedef std::uint16_t event_t;


		/**@brief attach a subscriber to the subject to listen for events
		 * @param [in] observer the observer you want to attach
		 */
		void attach(class subscriber_base* observer) const
		{
			m_views.push_back(observer);
		}

		void detach(class subscriber_base* observer) const
		{
			for (std::size_t i = 0; i < m_views.size(); i++) {
				if (m_views[i] == observer) {
					m_views[i] = m_views.back();
					m_views.pop_back();
					break;
				}
			}
		}

	protected:

		/**@brief notify all observers/subscribers of an event
		 * @param [in] e the event you want to inform all the subscribers of
		 */
		void notify(event_t e);

		/**@brief notify the first observer/subscriber of an event.
		 * Note that attached observers are pushed to the back!
		 * @param [in] e the event you want to inform the subscriber of
		 */
		void notifyFirst(event_t e);

		/**@brief notify the last observer/subscriber of an event.
		 * Note that attached observers are pushed to the back!
		 * @param [in] e the event you want to inform the subscriber of
		 */
		void notifyLast(event_t e);

		/**@brief notify the next observer/subscriber of an event.
		 * (starts at 0, only increases when this function is used)
		 * @param [in] e the event you want to inform the subscriber of
		 */
		void notifyNext(event_t e);

	private:
		mutable std::vector<class subscriber_base*> m_views;
		std::size_t m_iter = 0;
	};


}