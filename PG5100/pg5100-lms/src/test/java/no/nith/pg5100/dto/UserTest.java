package no.nith.pg5100.dto;

import org.junit.Before;
import org.junit.Test;

import javax.validation.ConstraintViolation;
import javax.validation.Validation;
import javax.validation.Validator;

import java.util.Set;

import static org.junit.Assert.*;

public class UserTest {
    private Validator validator;

    @Before
    public void setUp() throws Exception {
        validator = Validation.buildDefaultValidatorFactory().getValidator();
    }

    @Test
    public void nullValuesUser() throws Exception {
        User user = new User();

        Set<ConstraintViolation<User>> violations = validator.validate(user);
        assertEquals(3, violations.size());
    }

    @Test
    public void invalidUser() throws Exception {
        User user = new User();
        user.setEmail("test");
        user.setPassword("a");

        Set<ConstraintViolation<User>> violations = validator.validate(user);
        assertEquals(2, violations.size());
    }

    @Test
    public void validUser() throws Exception {
        User user = new User();
        user.setEmail("test@test.com");
        user.setPassword("abC123");
        user.setUserType(UserType.STUDENT);

        Set<ConstraintViolation<User>> violations = validator.validate(user);
        assertTrue(violations.isEmpty());
    }
}