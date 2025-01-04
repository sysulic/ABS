(define (problem tireworld-prob01)
(:domain tyreworld)
(:objects 
	w1 - wheel
	w2 - wheel 
	w3 - wheel 
	w4 - wheel
)

(:init
	(flat w1)
	(flat w2)
	(flat w3)
	(flat w4)
	(fastened w1)
	(fastened w2)
	(fastened w3)
	(fastened w4)
)
(:goal(and
	(inflated w1)
	(inflated w2)
	(inflated w3)
	(inflated w4)
	(fastened w1)
	(fastened w2)
	(fastened w3)
	(fastened w4))
)
)