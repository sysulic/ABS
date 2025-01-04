(define (domain tyreworld)
(:requirements :strips :typing)
(:types
    wheel - object)
(:predicates 
             (flat ?x - wheel)
             (inflated ?x - wheel)
             (fastened ?x - wheel)
             (have ?x - wheel)
)

(:action remove-wheel
    :parameters (?x - wheel)
    :precondition (and (flat ?x) 
                    (fastened ?x))
    :effect (and (have ?x)
              (not (fastened ?x)))
)

(:action put-on-wheel
    :parameters (?x - wheel)
    :precondition (and (have ?x) 
                  (inflated ?x))
    :effect (and (not (have ?x))
                  (fastened ?x))
)

(:action inflate
    :parameters (?x - wheel)
    :precondition (and (have ?x) 
            (flat ?x))
    :effect (and (inflated ?x)
            (not (flat ?x)))
)
)


