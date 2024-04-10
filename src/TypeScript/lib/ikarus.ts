export enum PointType {
  pntStart = 12,
  pntAngle,
  pntCurve,
  pntTangt,
}

export enum Pen {
  UP = 0,
  DOWN = 1,
}

export class Knot {
  x: number;
  y: number;
  type: PointType;
  c: number; // cosine of tangent
  s: number; // sine of tangent
  g: number; // auxiliary variable
  c0: number; // cosine of cord
  s0: number; // sine of cord
  ds: number; // distance of two points
  selected?: boolean; // auxiliary variable

  constructor() {
    this.x = 0;
    this.y = 0;
    this.type = PointType.pntStart;
    this.c = 0;
    this.s = 0;
    this.g = 0;
    this.c0 = 0;
    this.s0 = 0;
    this.ds = 0;
  }
}

export enum ResultSegmentTypes {
  GeradeM = "GeradeM",
  KreisM = "KreisM",
}

export type ResultSegment = GeradeMResult | KreisMResult;

export interface GeradeMResult {
  type: ResultSegmentTypes;
  x: number;
  y: number;
  pen: Pen;
  reduceFactor: number;
}

function GeradeM(
  x: number,
  y: number,
  pen: Pen,
  reduceFactor: number
): GeradeMResult {
  return { type: ResultSegmentTypes.GeradeM, x, y, pen, reduceFactor };
}

export interface KreisMResult {
  type: ResultSegmentTypes;
  xa: number;
  ya: number;
  x: number;
  y: number;
  xm2: number;
  ym2: number;
  r: number;
  sh: number;
  reduceFactor: number;
}

function KreisM(
  xa: number,
  ya: number,
  x: number,
  y: number,
  xm2: number,
  ym2: number,
  r: number,
  sh: number,
  reduceFactor: number
): KreisMResult {
  return {
    type: ResultSegmentTypes.KreisM,
    xa,
    ya,
    x,
    y,
    xm2,
    ym2,
    r,
    sh,
    reduceFactor,
  };
}

export function BuchM(
  ianf: number,
  iend: number,
  istart: number,
  iktot: number,
  reduceFactor: number,
  knots: Knot[]
): ResultSegment[] {
  const result: ResultSegment[] = [];

  let pen = Pen.UP;
  result.push(GeradeM(knots[ianf].x, knots[ianf].y, pen, reduceFactor));

  let i = ianf + 1;

  while (i <= iend) {
    let pi = knots[i];

    if (pi.type !== PointType.pntCurve) {
      // Non-curvepoints are treated as endpoints of straight lines
      if (pi.type === PointType.pntStart) {
        // Start point
        pen = Pen.UP;
        istart = i;
      } else {
        pen = Pen.DOWN;
      }

      result.push(GeradeM(pi.x, pi.y, pen, reduceFactor));
    } else {
      pen = Pen.DOWN;
      let m = i - 1;

      // calculate length of curve
      while (i < iend) {
        i++;
        if (knots[i].type !== PointType.pntCurve) {
          break;
        }
      }

      if (
        knots[i].type < PointType.pntAngle ||
        knots[i].type > PointType.pntTangt
      ) {
        i--;
      }

      let n = i;
      if (n - m <= 1) {
        // curve is represented by straight line
        result.push(GeradeM(knots[n].x, knots[n].y, pen, reduceFactor));
      } else {
        SplinM(
          knots,
          m,
          n,
          istart,
          iktot
        ); /* calculate tangents in curvepoints*/
        result.push(...SaurM(m, n, reduceFactor, knots)); /* calculate arcs */
      }
    }

    i++;
  }

  return result;
}

function SplinM(
  knots: Knot[],
  ianf: number,
  iend: number,
  istart: number,
  iktot: number
): void {
  // Initial setup and VektoM call
  VektoM(knots, ianf, iend, istart, iktot);

  // Variables for algorithm
  let n1 = iend - 1;
  let pn1 = knots[n1];

  let ka: number;
  let ds: number, z: number, h2: number, c2: number, s2: number;
  let h1: number = 0,
    c1: number = 0,
    s1: number = 0,
    pk1: Knot;

  pk1 = new Knot();

  // Setting starting values based on the first point type
  let panf = knots[ianf];
  let pend = knots[iend];

  switch (panf.type) {
    case PointType.pntTangt:
      ka = ianf + 1;
      panf.g = 0;
      panf.c = knots[ianf - 1].c0;
      panf.s = knots[ianf - 1].s0;
      break;
    case PointType.pntCurve:
      ka = ianf + 1;
      panf.g = 0;
      break;
    default:
      ka = ianf;
      panf.g = 0.5;
      panf.c = 1.5 * panf.c0; // Adjusting curvature at the end to 0
      panf.s = 1.5 * panf.s0;
      break;
  }

  // Main loop to solve the linear system of equations (Gauss-Elimination for tridiagonal systems)
  let k: number;
  let pk: Knot;

  for (k = ianf, pk = panf; k < iend; k++, pk = knots[k]) {
    // let pk = knots[ianf + k];
    ds = pk.ds;

    if (ds > 0.1) h2 = 1.0 / ds;
    else h2 = 1.0; // Assuming to prevent division by zero

    c2 = 3.0 * pk.c0 * h2;
    s2 = 3.0 * pk.s0 * h2;

    if (k !== ianf) {
      z = 1.0 / (2.0 * (h1 + h2) - h1 * pk1.g);

      pk.g = z * h2;
      pk.c = z * (c1 + c2 - h1 * pk1.c);
      pk.s = z * (s1 + s2 - h1 * pk1.s);

      if (k === n1) {
        // Special handling for the last point before the end
        if (pend.type === PointType.pntTangt) {
          pend.c = pend.c0;
          pend.s = pend.s0;

          pk.c -= pk.g * pend.c;
          pk.s -= pk.g * pend.s;

          break;
        } else {
          if (
            pend.type === PointType.pntCurve &&
            iend !== iktot &&
            knots[iend + 1].type >= PointType.pntAngle
          ) {
            pk.c -= pk.g * pend.c;
            pk.s -= pk.g * pend.s;
            break;
          }
        }

        z = 1.0 / (2.0 - knots[n1].g);

        pend.c = z * (3.0 * knots[n1].c0 - knots[n1].c);
        pend.s = z * (3.0 * knots[n1].s0 - knots[n1].s);
      }
    }

    c1 = c2;
    s1 = s2;
    h1 = h2;
    pk1 = pk;
  }

  // Backward substitution to solve for c and s
  while (true) {
    c1 = pk.c;
    s1 = pk.s;

    h2 = Math.sqrt(c1 * c1 + s1 * s1);

    if (h2 !== 0) h2 = 1.0 / h2;
    else h2 = 1.0;

    pk.c = c1 * h2;
    pk.s = s1 * h2;

    k--;

    if (k < ka) {
      break;
    }
    pk = knots[k];
    pk.c -= pk.g * c1;
    pk.s -= pk.g * s1;
  }

  // Special treatment for the endpoint
  // This might include setting the curvature for closed loops or adjusting angles
  // Implement based on specific needs for endpoint handling

  switch (pend.type /* special treatment of the endpoint */) {
    case PointType.pntCurve:
      if (pend.x !== panf.x || pend.y !== panf.y) break;

      const dxk = panf.c + pend.c;
      const dyk = panf.s + pend.s;

      h2 = Math.sqrt(dxk * dxk + dyk * dyk);

      if (h2 === 0.0) break;

      h2 = 1.0 / h2;

      panf.c = dxk * h2;
      panf.s = dyk * h2;

      pend.c = panf.c;
      pend.s = panf.s;

      break;

    case PointType.pntAngle:
      // let pend = knots[iend ]

      knots[iend - 2].g = pend.c;
      pn1.g = pend.s;

      if (iend !== iktot && knots[iend + 1]?.type >= PointType.pntAngle) break;

      if (pend.x !== panf.x || pend.y !== panf.y) break;

      pend.c = knots[istart].c;
      pend.s = knots[istart].s;
  }

  KottaM(knots, ianf, iend); // Modify tangents if necessary
}

function KottaM(knots: Knot[], m: number, n: number): void {
  let pm = knots[m];
  let pm1 = knots[m + 1];
  let pi: Knot = new Knot();
  let inResult: number, im: number;
  let c1: number, s1: number, c2: number, s2: number, c3: number, s3: number;
  let cs: number, ss: number, css: number, sss: number;
  let sa11: number = 0,
    sa22: number = 0,
    ca11: number = 0,
    ca22: number = 0;

  let sa1: number = 0;
  let sa2: number = 0;
  let ca1: number = 0;
  let ca2: number = 0;

  c1 = pm.c;
  s1 = pm.s;
  c2 = pm1.c;
  s2 = pm1.s;
  cs = pm.c0;
  ss = pm.s0;

  // Unpacking the result of DraikM directly into variables
  let result = DraikM(cs, ss, c1, s1, c2, s2, sa1, sa2, ca1, ca2);
  sa1 = result.sa1;
  sa2 = result.sa2;
  ca1 = result.ca1;
  ca2 = result.ca2;
  inResult = result.inVal;

  for (let i = m + 1; i < n; i++) {
    pi = knots[i];

    if (knots[i + 1].type !== PointType.pntAngle) {
      c3 = knots[i + 1].c;
      s3 = knots[i + 1].s;
    } else {
      c3 = knots[i - 1].g;
      s3 = knots[i].g;
    }

    css = pi.c0;
    sss = pi.s0;

    // Adjusting based on DraikM's result for the next segment
    result = DraikM(css, sss, c2, s2, c3, s3, sa11, sa22, ca11, ca22);

    sa11 = result.sa1;
    sa22 = result.sa2;
    ca11 = result.ca1;
    ca22 = result.ca2;
    im = result.inVal;

    if (inResult !== 0) {
      if (im !== 0) {
        if (inResult * im >= 0) {
          const { corr, pci, psi } = AusglM(
            inResult,
            sa1,
            sa2,
            sa11,
            sa22,
            c2,
            s2
          );
          c2 = pci;
          s2 = psi;

          if (corr !== 0) {
            result = DraikM(css, sss, c2, s2, c3, s3, sa11, sa22, ca11, ca22);
            sa11 = result.sa1;
            sa22 = result.sa2;
            ca11 = result.ca1;
            ca22 = result.ca2;

            pi.c = c2;
            pi.s = s2;
          }
        }
      } else {
        c2 = css;
        s2 = sss;

        c3 = css;
        s3 = sss;

        pi.c = c2;
        pi.s = s2;

        knots[i + 1].c = c3;
        knots[i + 1].s = s3;
      }
    }

    // Update variables for the next iteration
    inResult = im;

    sa1 = sa11;
    sa2 = sa22;

    ca1 = ca11;
    ca2 = ca22;

    cs = css;
    ss = sss;

    c1 = c2;
    s1 = s2;

    c2 = c3;
    s2 = s3;
  }

  // Final corrections
  pi.c = c2;
  pi.s = s2;
  if (pi.type !== PointType.pntCurve) {
    knots[n - 2].g = c2;
    knots[n - 1].g = s2;
  }
}

function DraikM(
  cs: number,
  ss: number,
  ci1: number,
  si1: number,
  ci: number,
  si: number,
  // ---------------------
  sa1: number,
  sa2: number,
  ca1: number,
  ca2: number
): {
  sa1: number;
  sa2: number;
  ca1: number;
  ca2: number;
  inVal: number;
} {
  // let sa1: number, sa2: number;
  let inResult: number;

  // Calculate sine of the 1st and 2nd angles
  sa1 = cs * si1 - ci1 * ss;
  sa2 = cs * si - ci * ss;

  // Determine the orientation of the angles and adjust signs accordingly
  if (sa1 > 0) {
    if (sa2 > 0) {
      const result = { sa1, sa2, ca1, ca2, inVal: 0 }; // Both angles are in the same direction

      return result;
    }
    sa2 = -sa2; // Adjust the sign of sa2
    inResult = -1; // Indicate orientation
  } else {
    if (sa2 < 0) {
      const result = { sa1, sa2, ca1, ca2, inVal: 0 }; // Both angles are in the same direction

      return result;
    }
    sa1 = -sa1; // Adjust the sign of sa1
    inResult = 1; // Indicate orientation
  }

  // Check if angles are too small to be considered
  if (sa1 < 0.0015 || sa2 < 0.0015) {
    const result = { sa1, sa2, ca1, ca2, inVal: 0 };

    return result;
  }

  // Calculate cosine of the 1st and 2nd angles
  ca1 = cs * ci1 + ss * si1;
  ca2 = cs * ci + ss * si;

  // Check if cosines are too small to be considered
  if (ca1 < 0.1 || ca2 < 0.1) {
    const result = { sa1, sa2, ca1, ca2, inVal: 0 };

    return result;
  }

  // Return the indicator of orientation and the sine and cosine of angles
  const result = { sa1, sa2, ca1, ca2, inVal: inResult };

  return result;
}

function AusglM(
  idreh: number,
  sa1: number,
  sa2: number,
  sa11: number,
  sa22: number,
  pci: number,
  psi: number
): {
  corr: number;
  pci: number;
  psi: number;
} {
  let dr: number = 0,
    del: number = 0,
    del1: number = 0,
    del2: number = 0,
    deldr: number = 0,
    c: number = 0,
    cdel: number = 0,
    cdels: number = 0;

  dr = idreh * 0.7;

  del1 = (sa1 - sa2) * dr;
  del2 = (sa11 - sa22) * dr;

  del = (del1 + del2) * 0.7;
  deldr = del * dr;

  // Check the condition for positive or negative rotation
  if (deldr >= 0) {
    if (deldr < 0.01) {
      // return 0; // No correction needed
      return { corr: 0, pci, psi };
    }

    if (deldr > del2 * dr) {
      del = del2; // Adjust for sa11 getting smaller
    }
  } else {
    if (deldr > -0.01) {
      // return 0; // No correction needed
      return { corr: 0, pci, psi };
    }

    if (deldr < del1 * dr) {
      del = del1; // Adjust for sa2 getting smaller
    }
  }

  // Perform the rotation correction
  cdels = 1.0 - del * del;
  cdel = Math.sqrt(cdels); // Calculate corrected direction

  c = pci * cdel - psi * del; // Rotate (pci, psi)

  psi = pci * del + psi * cdel;
  pci = c;

  return { corr: 1, pci, psi }; // Correction executed, return new direction
}

function SaurM(
  m: number,
  n: number,
  reduceFactor: number,
  knotList: Knot[]
): ResultSegment[] {
  const segments = [];
  let nn1 = Pen.DOWN;

  // Loop handling all IK-points
  for (let i = m + 1; i <= n; i++) {
    let kurve = true;
    let j = i - 1; // index of previous point

    // Calculate parameters of angle
    let result = DraikM(
      knotList[j].c0,
      knotList[j].s0,
      knotList[j].c,
      knotList[j].s,
      knotList[i].c,
      knotList[i].s,
      0,
      0,
      0,
      0
    );
    const sa = result.sa1;
    const se = result.sa2;
    const ca = result.ca1;
    const ce = result.ca2;
    const inVal = result.inVal;

    if (inVal === 0) {
      kurve = false; // If it is a small angle, draw a straight line
    } else {
      // Else calculate arcs
      let dr = (1 - ce) / (1 - ca);
      let r2 = knotList[j].ds / (dr * sa + se); // Radius of 2nd circle
      let sh = r2 * (1 - ce); // Cord height

      if (sh < reduceFactor) {
        kurve = false; // If cord height is small, straight line
      } else {
        let r1 = dr * r2;

        if (r1 < 1e-5 && r1 > -1e-5) {
          kurve = false; // Small radius, straight line
        } else {
          if (inVal < 0) {
            r1 = -r1;
            r2 = -r2;
          }

          // Calculate center points and auxiliary points
          let xm1 = knotList[j].x - r1 * knotList[j].s;
          let ym1 = knotList[j].y + r1 * knotList[j].c;

          let xm2 = knotList[i].x - r2 * knotList[i].s;
          let ym2 = knotList[i].y + r2 * knotList[i].c;

          let xh1 = xm1 + r1 * knotList[j].s0;
          let yh1 = ym1 - r1 * knotList[j].c0;

          let xa = knotList[j].x;
          let ya = knotList[j].y;

          // Draw first arc
          segments.push(
            KreisM(xa, ya, xh1, yh1, xm1, ym1, r1, sh, reduceFactor)
          );

          xa = xh1;
          ya = yh1;

          // Draw second arc
          segments.push(
            KreisM(
              xa,
              ya,
              knotList[i].x,
              knotList[i].y,
              xm2,
              ym2,
              r2,
              sh,
              reduceFactor
            )
          );
        }
      }
    }
    if (!kurve) {
      segments.push(GeradeM(knotList[i].x, knotList[i].y, nn1, reduceFactor));
    }
  }
  return segments;
}

function VektoMCalc(pknot: Knot, x: number, y: number): void {
  const dx = x - pknot.x;
  const dy = y - pknot.y;
  const ds = Math.sqrt(dx * dx + dy * dy);

  pknot.ds = ds;

  if (ds === 0.0) {
    pknot.c0 = 1.0;
    pknot.s0 = 0.0;
  } else {
    pknot.c0 = dx / ds;
    pknot.s0 = dy / ds;
  }
}

function VektoM(
  knotList: Knot[],
  ianf: number,
  iend: number,
  istart: number,
  iktot: number
): void {
  if (knotList[ianf].type === PointType.pntTangt) {
    ianf--;
  }

  for (let i = ianf; i < iend; i++) {
    const pi = knotList[i];
    const pi1 = knotList[i + 1];
    VektoMCalc(pi, pi1.x, pi1.y);
  }

  if (knotList[iend].type === PointType.pntTangt) {
    let pi1: Knot;
    if (iend === iktot && knotList[iend + 1].type < PointType.pntAngle) {
      pi1 = knotList[istart + 1];
    } else {
      pi1 = knotList[iend + 1];
    }
    VektoMCalc(knotList[iend], pi1.x, pi1.y);
  }
}
