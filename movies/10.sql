SELECT name FROM directors, movies, ratings, people
WHERE ratings.movie_id = movies.id
AND directors.person_id = people.id
AND directors.movie_id = movies.id
AND rating >= 9
GROUP BY birth;
