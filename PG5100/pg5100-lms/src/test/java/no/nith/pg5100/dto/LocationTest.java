package no.nith.pg5100.dto;

import org.junit.Before;
import org.junit.Test;

import javax.validation.ConstraintViolation;
import javax.validation.Validation;
import javax.validation.Validator;

import java.util.Set;

import static org.junit.Assert.*;

public class LocationTest {
    private Validator validator;

    @Before
    public void setUp() throws Exception {
        validator = Validation.buildDefaultValidatorFactory().getValidator();
    }

    @Test
    public void invalidLocation() throws Exception {
        Location location = new Location();
        Set<ConstraintViolation<Location>> violations = validator.validate(location);
        assertEquals(2, violations.size());
    }

    @Test
    public void validLocation() throws Exception {
        Location location = new Location();
        location.setBuilding("Galleriet");
        location.setRoom("82");
        Set<ConstraintViolation<Location>> violations = validator.validate(location);
        assertTrue(violations.isEmpty());
    }
}