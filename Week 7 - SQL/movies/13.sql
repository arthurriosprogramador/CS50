SELECT DISTINCT people.name
FROM movies, people, stars, stars AS skb, people AS kb
WHERE kb.name = 'Kevin Bacon'
AND kb.birth = '1958'
AND skb.person_id = kb.id
AND movies.id = skb.movie_id
AND stars.movie_id = movies.id
AND people.id = stars.person_id
AND people.id != kb.id;