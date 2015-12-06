INSERT INTO User (id, email, password, usertype) VALUES (1, 'mari@test.com', '123ABc', 'STUDENT');
INSERT INTO User (id, email, password, usertype) VALUES (2, 'ole@test.com', 'abC123', 'TEACHER');
INSERT INTO User (id, email, password, usertype) VALUES (3, 'kari@test.com', '1234Abc', 'STUDENT');
INSERT INTO User (id, email, password, usertype) VALUES (4, 'marianne@test.com', '1234Abc', 'STUDENT');

INSERT INTO Location (id, building, room) VALUES (1, 'Galleriet', '82');
INSERT INTO Location (id, building, room) VALUES (2, 'Galleriet', '81');

INSERT INTO Subject (id, name, FK_LOCATION) VALUES (1, 'PG5100', 1);
INSERT INTO Subject (id, name, FK_LOCATION) VALUES (2, 'PG6100', 2);

INSERT INTO USR_SUB (SUBJECTS_ID, USERS_ID) VALUES (1, 1);
INSERT INTO USR_SUB (SUBJECTS_ID, USERS_ID) VALUES (2, 1);
INSERT INTO USR_SUB (SUBJECTS_ID, USERS_ID) VALUES (1, 2);
INSERT INTO USR_SUB (SUBJECTS_ID, USERS_ID) VALUES (2, 3);

INSERT INTO EVENT (id, title, eventType, description, FK_SUBJECT) VALUES (1, 'Repetisjonsforelesning',  'LECTURE', 'One morning, when Gregor Samsa woke from troubled dreams', 1);
INSERT INTO EVENT (id, title, eventType, description, FK_SUBJECT) VALUES (2, 'Gjesteforelesning', 'LECTURE', 'It wasn''t a dream. His room, a proper human room although a little too small', 2);
INSERT INTO EVENT (id, title, eventType, description, FK_SUBJECT) VALUES (3, 'Mekkekveld med Listog', 'LECTURE', 'For instance, on the planet Earth, man had always assumed that he was more intelligent than dolphins', 2);
INSERT INTO EVENT (id, title, eventType, description, FK_SUBJECT) VALUES (4, 'Egentrening', 'TRAINING', 'Far out in the uncharted backwaters of the unfashionable end of the western spiral arm of the Galaxy', 2);


INSERT INTO EVENT_DETAILS (event_id, startDateTime, endDateTime) VALUES (1, '2015-10-02 14:15:00', '2015-11-02 15:00:00' );
INSERT INTO EVENT_DETAILS (event_id, startDateTime, endDateTime) VALUES (2, '2015-10-02 16:30:00', '2015-11-02 19:00:00' )
INSERT INTO EVENT_DETAILS (event_id, startDateTime, endDateTime) VALUES (3, '2015-11-16 19:20:00', '2015-11-16 20:00:00' );
INSERT INTO EVENT_DETAILS (event_id, startDateTime, endDateTime) VALUES (4, '2015-12-02 14:15:00', '2015-12-02 15:00:00' );
