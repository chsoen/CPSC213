package arch.sm213.machine.student;

import jdk.jfr.internal.tool.Main;
import machine.AbstractMainMemory;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

class MainMemoryTest {
    private MainMemory mainMemory;

    @BeforeEach
    void setup() {
        mainMemory = new MainMemory(64);
    }

    @Test
    void isAccessAligned() {
        assertTrue(mainMemory.isAccessAligned(0x8df7000, 8));
        assertTrue(mainMemory.isAccessAligned(0xff00, 4));
        assertTrue(mainMemory.isAccessAligned(0xabdef0, 2));

        assertFalse(mainMemory.isAccessAligned(0xabd01a, 8));
        assertFalse(mainMemory.isAccessAligned(0xff01, 4));
        assertFalse(mainMemory.isAccessAligned(0xabdef5, 2));
    }

    @Test
    void bytesToInteger() {
        int expected = mainMemory.bytesToInteger((byte) 0x78, (byte) 0x7f, (byte) 0xd8, (byte) 0xcf);
        assertEquals(2021644495, expected);

        expected = mainMemory.bytesToInteger((byte) 0x36, (byte) 0xbe, (byte) 0x6f, (byte) 0x9e);
        assertEquals(918450078, expected);
    }

    @Test
    void integerToBytes() {
        byte[] expected = mainMemory.integerToBytes(2021644495);
        assertEquals((byte) 0x78, expected[0]);
        assertEquals((byte) 0x7f, expected[1]);
        assertEquals((byte) 0xd8, expected[2]);
        assertEquals((byte) 0xcf, expected[3]);

        expected = mainMemory.integerToBytes(918450078);
        assertEquals((byte) 0x36, expected[0]);
        assertEquals((byte) 0xbe, expected[1]);
        assertEquals((byte) 0x6f, expected[2]);
        assertEquals((byte) 0x9e, expected[3]);
    }

    @Test
    void setAndGetTest() {
        byte[] value = mainMemory.integerToBytes(2021644495);
        byte[] value2 = mainMemory.integerToBytes(918450078);
        try {
            mainMemory.set(0, value);
            assertArrayEquals(value, mainMemory.get(0,value.length));

            mainMemory.set(mainMemory.length()-4, value2);
            assertArrayEquals(value2, mainMemory.get(mainMemory.length()-4,value.length));
            // pass
        } catch (AbstractMainMemory.InvalidAddressException e) {
            System.out.println("Expected nothing, caught InvalidAddressException");
            fail();
        }

        try {
            mainMemory.set(mainMemory.length()-3, value);
            System.out.println("Expected InvalidAddressException, caught nothing");
            fail();
        } catch (AbstractMainMemory.InvalidAddressException e) {
            // pass
        }

        try {
            mainMemory.get(mainMemory.length()-3, value.length);
            System.out.println("Expected InvalidAddressException, caught nothing");
            fail();
        } catch (AbstractMainMemory.InvalidAddressException e) {
            // pass
        }
    }
}