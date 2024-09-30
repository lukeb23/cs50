SELECT name FROM people, stars
WHERE people.id = stars.person_id
AND movie_id = (SELECT id FROM movies WHERE title= "Toy Story");