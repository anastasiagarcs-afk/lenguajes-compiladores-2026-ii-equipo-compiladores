#!/usr/bin/env python3
"""Pruebas unitarias del Ejercicio 1 - Caballo de ajedrez."""
import unittest

from caballo import validar, parsear_casilla


class TestParsearCasilla(unittest.TestCase):
    def test_valida(self):
        self.assertEqual(parsear_casilla("a1"), (0, 0))
        self.assertEqual(parsear_casilla("h8"), (7, 7))
        self.assertEqual(parsear_casilla("E4"), (4, 3))

    def test_invalida(self):
        self.assertIsNone(parsear_casilla("i1"))
        self.assertIsNone(parsear_casilla("a9"))
        self.assertIsNone(parsear_casilla("a0"))
        self.assertIsNone(parsear_casilla("a"))
        self.assertIsNone(parsear_casilla("zz"))


class TestValidar(unittest.TestCase):
    def test_secuencia_correcta_ejemplo_pdf(self):
        ok, msg = validar("a1", "Kc2 Kd4 Kb3 Kc5".split())
        self.assertTrue(ok)
        self.assertEqual(msg, "movimiento CORRECTO")

    def test_salto_sencillo(self):
        ok, msg = validar("a1", ["Kb3"])
        self.assertTrue(ok)
        self.assertEqual(msg, "movimiento CORRECTO")

    def test_secuencia_desde_h8(self):
        ok, msg = validar("h8", "Kf7 Kd6".split())
        self.assertTrue(ok)
        self.assertEqual(msg, "movimiento CORRECTO")

    def test_letra_distinta_de_k(self):
        ok, msg = validar("a1", ["Be2"])
        self.assertFalse(ok)
        self.assertIn("no pertenece a un caballo", msg)
        self.assertIn("movimiento 1", msg)

    def test_letra_distinta_de_k_posterior(self):
        ok, msg = validar("a1", "Kc2 Rd4".split())
        self.assertFalse(ok)
        self.assertIn("no pertenece a un caballo", msg)
        self.assertIn("movimiento 2", msg)

    def test_casilla_invalida(self):
        ok, msg = validar("a1", ["Kh9"])
        self.assertFalse(ok)
        self.assertIn("casilla invalida", msg)

    def test_primer_fallo_falla_en_movimiento_1(self):
        ok, msg = validar("a1", ["Kc3"])
        self.assertFalse(ok)
        self.assertEqual(msg, "INCORRECTO falla en el movimiento 1")

    def test_primer_fallo_falla_en_movimiento_2(self):
        ok, msg = validar("a1", "Kc2 Kc4".split())
        self.assertFalse(ok)
        self.assertEqual(msg, "INCORRECTO falla en el movimiento 2")

    def test_posicion_inicial_invalida(self):
        ok, msg = validar("z9", ["Kc2"])
        self.assertFalse(ok)
        self.assertIn("posicion inicial invalida", msg)


if __name__ == "__main__":
    unittest.main(verbosity=2)
