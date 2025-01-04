(define (problem strips-log-x-1)
   (:domain logistics)
   (:objects package4 package3 package2 package1 - package
             truck4 truck3 truck2 truck1 - truck
             city2 city1 - city
             city2-1 city1-1 - location
             city2-2 city1-2 - airport 
             plane2 plane1 - airplane
             )
   (:init 
          (in-city city2-2 city2)
          (in-city city2-1 city2)
          (in-city city1-2 city1)
          (in-city city1-1 city1)
          (airplane-at plane2 city2-2)
          (airplane-at plane1 city1-2)
          (truck-at truck4 city2-1)
          (truck-at truck3 city2-1)
          (truck-at truck2 city1-1)
          (truck-at truck1 city1-1)
          (package-at package4 city1-1)
          (package-at package3 city1-1)
          (package-at package2 city1-1)
          (package-at package1 city1-1))
   (:goal (and 
               (package-at package4 city2-1)
               (package-at package3 city2-1)
               (package-at package2 city2-1)
               (package-at package1 city2-1))))