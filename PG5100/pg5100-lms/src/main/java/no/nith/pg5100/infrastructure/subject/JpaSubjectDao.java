package no.nith.pg5100.infrastructure.subject;

import no.nith.pg5100.dto.Subject;

import javax.ejb.Stateless;
import javax.persistence.EntityManager;
import javax.persistence.PersistenceContext;
import javax.persistence.TypedQuery;
import java.util.List;

@Stateless
public class JpaSubjectDao implements SubjectDao {

    @PersistenceContext(unitName = "pg5100-lms")
    private EntityManager entityManager;

    public JpaSubjectDao() {
    }

    JpaSubjectDao(EntityManager entityManager) {
        this.entityManager = entityManager;
    }

    @Override
    public Subject persist(Subject subject) {
        entityManager.persist(subject);
        return subject;
    }

    @Override
    public Subject findById(int id) {
        return entityManager.find(Subject.class, id);
    }

    @Override
    public List<Subject> getAll() {
        TypedQuery<Subject> query = entityManager.createNamedQuery("Subject.getAll", Subject.class);
        return query.getResultList();
    }
}
