(define (domain logistics)
  (:requirements :strips :typing)
  (:types 
    package - object
    truck - object
    airplane - object
    location - object
    airport - location
    city - object)
  (:predicates
    (in-city ?loc - location ?city - city)
    (package-at ?pac - package ?loc - location)
    (truck-at ?tru - truck ?loc - location)
    (airplane-at ?airplane - airplane ?loc - airport)
    (in-truck ?pac - package ?tru - truck)
    (in-airplane ?pac - package ?airplane - airplane)
    )

  (:action load-truck
    :parameters (?pac - package ?tru - truck ?loc - location)
    :precondition (and (package-at ?pac ?loc) (truck-at ?tru ?loc))
    :effect (and (not (package-at ?pac ?loc)) 
     (in-truck ?pac ?tru))
    )

  (:action load-airplane
    :parameters (?pac - package ?airplane - airplane ?loc - location)
    :precondition (and (package-at ?pac ?loc) (airplane-at ?airplane ?loc))
    :effect (and (not (package-at ?pac ?loc)) 
     (in-airplane ?pac ?airplane))
    )

  (:action unload-truck
    :parameters (?pac - package ?tru - truck ?loc - location)
    :precondition (and (in-truck ?pac ?tru) (truck-at ?tru ?loc))
    :effect (and (not (in-truck ?pac ?tru)) 
     (package-at ?pac ?loc))
    )

  (:action unload-airplane
    :parameters (?pac - package ?airplane - airplane ?loc - location)
    :precondition (and (in-airplane ?pac ?airplane) 
      (airplane-at ?airplane ?loc))
    :effect (and (not (in-airplane ?pac ?airplane)) 
     (package-at ?pac ?loc))
    )

  (:action drive-truck
    :parameters (?tru - truck 
      ?loc-from - location
      ?loc-to - location
      ?city - city)
    :precondition (and  (truck-at ?tru ?loc-from)
      (in-city ?loc-from ?city)
      (in-city ?loc-to ?city))
    :effect (and (not (truck-at ?tru ?loc-from)) 
     (truck-at ?tru ?loc-to)))

  (:action fly-airplane
    :parameters (?airplane - airplane
      ?loc-from - airport
      ?loc-to - airport)
    :precondition (airplane-at ?airplane ?loc-from)
    :effect (and (not (airplane-at ?airplane ?loc-from)) 
      (airplane-at ?airplane ?loc-to)))
  )
