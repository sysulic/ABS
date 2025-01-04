(define (problem prob03)
  (:domain gripper)
  (:objects 
    ball1 - ball
    ball2 - ball
    ball3 - ball
    ball4 - ball
    ball5 - ball
    ball6 - ball
    ball7 - ball
    ball8 - ball
    ball9 - ball
    ball10 - ball
    ball11 - ball
    ball12 - ball
    ball13 - ball
    ball14 - ball
    ball15 - ball
    ball16 - ball
    ball17 - ball
    ball18 - ball
    ball19 - ball
    ball20 - ball
    rooma - room
    roomb - room
    left - gripper
    right - gripper 
  )
  (:init
    (at ball1 rooma)
    (at ball2 rooma)
    (at ball3 rooma)
    (at ball4 rooma)
    (at ball5 rooma)
    (at ball6 rooma)
    (at ball7 rooma)
    (at ball8 rooma)
    (at ball9 rooma)
    (at ball10 rooma)
    (at ball11 rooma)
    (at ball12 rooma)
    (at ball13 rooma)
    (at ball14 rooma)
    (at ball15 rooma)
    (at ball16 rooma)
    (at ball17 rooma)
    (at ball18 rooma)
    (at ball19 rooma)
    (at ball20 rooma)
    (at-robby rooma)
    (free left)
    (free right)
  )
  (:goal (and 
          (at ball1 roomb)
          (at ball2 roomb)
          (at ball3 roomb)
          (at ball4 roomb)
          (at ball5 roomb)
          (at ball6 roomb)
          (at ball7 roomb)
          (at ball8 roomb)
          (at ball9 roomb)
          (at ball10 roomb)
          (at ball11 roomb)
          (at ball12 roomb)
          (at ball13 roomb)
          (at ball14 roomb)
          (at ball15 roomb)
          (at ball16 roomb)
          (at ball17 roomb)
          (at ball18 roomb)
          (at ball19 roomb)
          (at ball20 roomb)
        )  
  )
)
