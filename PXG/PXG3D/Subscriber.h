#pragma once
#include "Subject.h"

namespace PXG
{
/**@interface subscriber_base
 * @brief the observer/subscriber in the observer-pattern
 * should be used as a base-class to give an object
 * the ability to be notified by other classes
 */
 class subscriber_base
 {
 public:
	 virtual ~subscriber_base() = default;

	 /**@brief gets called when a subject notifies you of an event
	  * @param [in] subject_base the ptr to the subject that notified you
	  * @param [in] event the event that occured in the subject
	  */
	 virtual void onNotify(subject_base* subject_base, subject_base::event_t event) = 0;

	 /**@brief shortcut for subscribing to an subject
	  * use as follows
	  * @code
	  * ... // in the ctor etc...
	  * subscribe(my_subject);
	  * @endcode
	  */
	 void subscribe(const subject_base& subj) { subj.attach(this); }


	 /**@brief removes the subscription from a subject
	  * @param [in] subj the subject you want to detach from
	  */
	 void remove_subscription(const subject_base& subj) { subj.detach(this); }

 };
}
