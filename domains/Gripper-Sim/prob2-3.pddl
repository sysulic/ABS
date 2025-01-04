(define (problem prob02)
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
    roomc - room
    left1 - gripper
    right1 - gripper
    mid1 - gripper 
    left2 - gripper
    right2 - gripper
  )
  (:init
    (at ball1 rooma)
    (at ball2 rooma)
    (at ball3 rooma)
    (at ball4 rooma)
    (at ball5 rooma)
    (at ball6 rooma)
    (at ball7 roomb)
    (at ball8 roomb)
    (at ball9 roomb)
    (at ball10 roomb)
    (at ball11 roomb)
    (at ball12 roomb)
    (at ball13 roomc)
    (at ball14 roomc)
    (at ball15 roomc)
    (at ball16 roomc)
    (at ball17 roomc)
    (at ball18 roomc)
    (at ball19 roomc)
    (at ball20 roomc)
    (at-robby rooma)
    (free left1)
    (free right1)
    (free mid1)
    (free left2)
    (free right2)
  )
  (:goal (and 
          (at ball1 roomc)
          (at ball2 roomc)
          (at ball3 roomc)
          (at ball4 roomc)
          (at ball5 roomc)
          (at ball6 roomc)
          (at ball7 rooma)
          (at ball8 rooma)
          (at ball9 rooma)
          (at ball10 rooma)
          (at ball11 rooma)
          (at ball12 rooma)
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
