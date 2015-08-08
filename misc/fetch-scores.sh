#! /bin/bash

API_TOKEN=LDy+zmxegHDuVeh11iUiugmvJ3X5uyTmuvPs7PWSrHc=
curl -3 --user :$API_TOKEN -X GET https://davar.icfpcontest.org/teams/79/solutions | python -m json.tool

