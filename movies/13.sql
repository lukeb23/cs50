SELECT DISTINCT(name) FROM people, movies, stars
WHERE stars.movie_id = movies.id
AND stars.person_id = people.id
AND title IN
(SELECT title FROM movies, people, stars
WHERE stars.movie_id = movies.id
AND stars.person_id = people.id
AND name ="Kevin Bacon"
AND birth = "1958")
AND name IS NOT
(SELECT name FROM movies, stars, people
WHERE stars.movie_id = movies.id
AND stars.person_id = people.id
AND name = "Kevin Bacon"
AND birth = "1958");
