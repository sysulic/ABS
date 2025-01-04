(define (problem prob00-l2-c5)
(:domain ferry)
(:objects 
		  l0 - location
		  l1 - location
          c0 - car 
          c1 - car
          c2 - car 
          c3 - car 
          c4 - car
)
(:init
	(empty-ferry)
	(at c0 l0)
	(at c1 l0)
	(at c2 l0)
	(at c3 l0)
	(at c4 l0)
	(at-ferry l0)
)
(:goal
	(and
		(at c0 l1)
		(at c1 l1)
		(at c2 l1)
		(at c3 l1)
		(at c4 l1)
)
)
)


