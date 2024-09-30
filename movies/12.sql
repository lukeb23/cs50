SELECT title FROM movies, stars, people
WHERE stars.movie_id = movies.id
AND stars.person_id = people.id
AND name IN ("Johnny Depp", "Helena Bonham Carter")
GROUP BY title
HAVING COUNT(*) = 2;