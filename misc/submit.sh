#!/bin/bash
API_TOKEN=BoNF0I954Tpwq+EIm2YOYbPK+QO0+2qYJVfCcZLmuFk=
curl -ssl3 --user :$API_TOKEN -X POST -H "Content-Type: application/json" -d @- https://davar.icfpcontest.org/teams/79/solutions
