import fs from "fs";
import { Knot, BuchM } from "./lib/ikarus";

function processDigitalTypefaces() {
  const csv = fs.readFileSync("../../data/DigitalTypefaces.csv", "utf8");

  const knots: Knot[] = csv.split("\n").map((line) => {
    const [empty, type, x, y] = line.split("|");

    const newPoint = new Knot();
    newPoint.x = parseFloat(x);
    newPoint.y = parseFloat(y);
    newPoint.type = parseInt(type);
    return newPoint;
  });

  let ianf = 0;
  let iend = knots.length - 1;
  let istart = 0;
  let iktot = knots.length;
  let reduceFactor = 1;

  const result = BuchM(ianf, iend, istart, iktot, reduceFactor, knots);

  fs.writeFileSync(
    "../../data/DigitalTypefaces_TypeScript.json",
    JSON.stringify(result, null, 2),
  );
}

function processIkarusNimbusSansRegular() {
  const csv = fs.readFileSync("../../data/IkarusNimbusSansRegular.csv", "utf8");

  const knots: Knot[] = csv.split("\n").filter((line) => !line.startsWith("#")).map((line) => {
    const [empty, type, x, y] = line.split(" ");

    let parsedType: number;

    switch (type) {
      case "Start":
        parsedType = 12;
        break;
      case "Corner":
        parsedType = 13;
        break;
      case "Curve":
        parsedType = 14;
        break;
      case "Tangent":
        parsedType = 15;
        break;
      default:
        throw new Error(`Invalid type: ${type}`);
    }

    const newPoint = new Knot();
    newPoint.x = parseFloat(x);
    newPoint.y = parseFloat(y);
    newPoint.type = parsedType;
    return newPoint;
  });

  let ianf = 0;
  let iend = knots.length - 1;
  let istart = 0;
  let iktot = knots.length;
  let reduceFactor = 1;

  const result = BuchM(ianf, iend, istart, iktot, reduceFactor, knots);

  fs.writeFileSync(
    "../../data/IkarusNimbusSansRegular_TypeScript.json",
    JSON.stringify(result, null, 2),
  );
}

processDigitalTypefaces();
processIkarusNimbusSansRegular();
