package no.nith.pg5100.infrastructure.subject;

import no.nith.pg5100.dto.Subject;

import java.util.List;

public interface SubjectDao {
    Subject persist(Subject subject);
    Subject findById(int id);
    List<Subject> getAll();
}
