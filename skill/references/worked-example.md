
# Worked Example: Cross-System Concepts

## Why This Example

Most real projects are polyglot. A single business concept like
"payments" touches:

- **Go** — backend microservice (entity, use case, resolver)
- **Flutter/Dart** — mobile app (model, service, screen, state)
- **SQL** — database schema and migrations
- **Protobuf** — gRPC contracts between services
- **Shell** — deployment and operational scripts
- **GraphQL** — API contract between frontend and backend

In traditional development, understanding "payments" requires reading
fragments scattered across multiple repositories, directories, and
languages. The "why" behind each piece exists only in the programmer's
head.

In literate programming, **one `.weft` file tells the entire story**.



## The Concept File: `payments.weft`

Below is a complete concept file for "Payments" that crosses six
technologies. Each subsection explains one facet of the concept,
followed by the code chunk that implements it.

### The Domain Model (shared understanding)

Before writing code in any language, we define what a payment IS.
This prose serves as the single conceptual reference for all systems:

A payment represents a transfer of funds from a user to a merchant.
It has an amount, a currency, a status that progresses through a
lifecycle (pending, processing, approved, rejected, fraud_detected),
and timestamps for auditing. The user initiates the payment in the
mobile app; the backend processes it; the fraud detector evaluates it.

### The Database (SQL)

The database is the source of truth for payment state. We define the
migration first because it establishes the constraints that all other
systems must respect:

```
@d Payments: migration @{
CREATE TYPE payment_status AS ENUM (
    'pending', 'processing', 'approved', 'rejected', 'fraud_detected'
);

CREATE TABLE payments (
    id          BIGSERIAL PRIMARY KEY,
    user_id     BIGINT NOT NULL REFERENCES users(id),
    merchant_id BIGINT NOT NULL REFERENCES merchants(id),
    amount      NUMERIC(12,2) NOT NULL CHECK (amount > 0),
    currency    VARCHAR(3) NOT NULL DEFAULT 'USD',
    status      payment_status NOT NULL DEFAULT 'pending',
    created_at  TIMESTAMPTZ NOT NULL DEFAULT now(),
    updated_at  TIMESTAMPTZ NOT NULL DEFAULT now()
);

CREATE INDEX idx_payments_user ON payments(user_id);
CREATE INDEX idx_payments_status ON payments(status);
@}
```

Note the `CHECK (amount > 0)` constraint. This business rule lives
in the database, not in application code, because no code path should
ever bypass it. The narrative explains this decision in context.

### Seed Data (SQL)

For development and testing, we need realistic sample data. The seed
script inserts payments in various states so that every screen and
query can be tested:

```
@d Payments: seed data @{
INSERT INTO payments (user_id, merchant_id, amount, currency, status)
VALUES
    (1, 10, 49.99, 'USD', 'approved'),
    (1, 11, 150.00, 'EUR', 'pending'),
    (2, 10, 9.99, 'USD', 'rejected'),
    (3, 12, 500.00, 'USD', 'fraud_detected');
@}
```



### The GraphQL Contract

This schema defines the API contract between the Flutter app and the
Go backend. We place it here, between the two systems that consume it,
so that any change is immediately visible in context with both producer
and consumer:

```
@d Payments: schema GraphQL @{
enum PaymentStatus {
    PENDING
    PROCESSING
    APPROVED
    REJECTED
    FRAUD_DETECTED
}

type Payment @key(fields: "id") {
    id: ID!
    amount: Float!
    currency: String!
    status: PaymentStatus!
    createdAt: DateTime!
}

input CreatePaymentInput {
    amount: Float!
    currency: String!
    merchantId: ID!
}

extend type Mutation {
    createPayment(input: CreatePaymentInput!): Payment!
}

extend type Query {
    payment(id: ID!): Payment
    myPayments(status: PaymentStatus): [Payment!]!
}
@}
```

The `@key(fields: "id")` directive enables Apollo Federation --- this
payment type can be referenced from other services. Notice that the
enum values match the PostgreSQL enum exactly; the narrative makes this
correspondence explicit.



### The Backend (Go)

Go receives the GraphQL mutation, validates the input, calls the fraud
detector via gRPC, and persists the payment. Each chunk below is a
layer of the backend, but they live together because they serve the
same concept.

**Entity:**

```
@d Payments: Go entity @{
package entity

import "time"

type PaymentStatus string

const (
    PaymentPending       PaymentStatus = "pending"
    PaymentProcessing    PaymentStatus = "processing"
    PaymentApproved      PaymentStatus = "approved"
    PaymentRejected      PaymentStatus = "rejected"
    PaymentFraudDetected PaymentStatus = "fraud_detected"
)

type Payment struct {
    ID         int64
    UserID     int64
    MerchantID int64
    Amount     float64
    Currency   string
    Status     PaymentStatus
    CreatedAt  time.Time
    UpdatedAt  time.Time
}
@}
```

**Use case** (the business logic that orchestrates validation, fraud
detection, and persistence):

```
@d Payments: Go use case @{
func (uc *PaymentUseCase) Create(ctx context.Context,
    userID int64, input CreatePaymentInput) (*Payment, error) {

    // 1. Build the payment entity
    payment := &entity.Payment{
        UserID:     userID,
        MerchantID: input.MerchantID,
        Amount:     input.Amount,
        Currency:   input.Currency,
        Status:     entity.PaymentPending,
    }

    // 2. Call fraud detector via gRPC
    result, err := uc.fraudClient.Evaluate(ctx, payment)
    if err != nil {
        return nil, fmt.Errorf("fraud check failed: %w", err)
    }
    if result == FraudRejected {
        payment.Status = entity.PaymentFraudDetected
    } else {
        payment.Status = entity.PaymentProcessing
    }

    // 3. Persist to PostgreSQL
    saved, err := uc.repo.Create(ctx, payment)
    if err != nil {
        return nil, fmt.Errorf("save payment: %w", err)
    }

    return saved, nil
}
@}
```

The use case makes the flow visible: validate, check fraud, persist.
A reader seeing this for the first time understands the complete backend
flow without jumping between files.



### The Frontend (Flutter/Dart)

The Flutter app lets the user initiate a payment and see its status.
The model mirrors the GraphQL type, the service calls the mutation,
and the screen presents the UI.

**Model:**

```
@d Payments: Flutter model @{
enum PaymentStatus { pending, processing, approved, rejected, fraudDetected }

class Payment {
  final String id;
  final double amount;
  final String currency;
  final PaymentStatus status;
  final DateTime createdAt;

  Payment({
    required this.id,
    required this.amount,
    required this.currency,
    required this.status,
    required this.createdAt,
  });

  factory Payment.fromJson(Map<String, dynamic> json) {
    return Payment(
      id: json['id'],
      amount: json['amount'].toDouble(),
      currency: json['currency'],
      status: PaymentStatus.values.byName(json['status']),
      createdAt: DateTime.parse(json['createdAt']),
    );
  }
}
@}
```

**Service** (calls the GraphQL mutation defined above):

```
@d Payments: Flutter service @{
class PaymentService {
  final GraphQLClient client;

  PaymentService(this.client);

  Future<Payment> createPayment({
    required double amount,
    required String currency,
    required String merchantId,
  }) async {
    final result = await client.mutate(
      MutationOptions(
        document: gql(r'''
          mutation CreatePayment($input: CreatePaymentInput!) {
            createPayment(input: $input) {
              id amount currency status createdAt
            }
          }
        '''),
        variables: {
          'input': {
            'amount': amount,
            'currency': currency,
            'merchantId': merchantId,
          },
        },
      ),
    );
    return Payment.fromJson(result.data!['createPayment']);
  }
}
@}
```

**Screen** (the payment confirmation UI):

```
@d Payments: Flutter screen @{
class PaymentScreen extends ConsumerWidget {
  final String merchantId;
  final double amount;
  final String currency;

  const PaymentScreen({
    required this.merchantId,
    required this.amount,
    required this.currency,
  });

  @override
  Widget build(BuildContext context, WidgetRef ref) {
    return Scaffold(
      appBar: AppBar(title: const Text('Confirm Payment')),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            Text('$currency \${amount.toStringAsFixed(2)}',
                style: Theme.of(context).textTheme.headlineLarge),
            const SizedBox(height: 24),
            ElevatedButton(
              onPressed: () => _processPayment(context, ref),
              child: const Text('Pay Now'),
            ),
          ],
        ),
      ),
    );
  }
}
@}
```

Notice that the Flutter model's enum values match the GraphQL enum
and the PostgreSQL enum. Because all three definitions live in the
same narrative, this consistency is visible and easy to maintain.



### The gRPC Contract and Fraud Detector (Protobuf + Python)

The fraud detection service runs as a separate Python process. The
gRPC contract connects it to the Go backend. We define the contract
here, between producer and consumer, so changes are visible to both:

**Protocol Buffer:**

```
@d Payments: proto gRPC @{
syntax = "proto3";
package payments.v1;

service FraudDetection {
  rpc Evaluate(PaymentRequest) returns (FraudResult);
}

message PaymentRequest {
  double amount = 1;
  string currency = 2;
  int64 user_id = 3;
  int64 merchant_id = 4;
}

message FraudResult {
  enum Decision {
    APPROVED = 0;
    REJECTED = 1;
    REVIEW_NEEDED = 2;
  }
  Decision decision = 1;
  double score = 2;
  string reason = 3;
}
@}
```

**Python fraud detector:**

```
@d Payments: Python fraud detector @{
import grpc
from payments.v1 import fraud_pb2, fraud_pb2_grpc

class FraudDetector(fraud_pb2_grpc.FraudDetectionServicer):
    AMOUNT_THRESHOLD = 1000.0
    VELOCITY_WINDOW = 3600  # seconds

    def Evaluate(self, request, context):
        score = self._compute_score(request)

        if score > 0.8:
            decision = fraud_pb2.FraudResult.REJECTED
            reason = "High fraud score"
        elif score > 0.5:
            decision = fraud_pb2.FraudResult.REVIEW_NEEDED
            reason = "Manual review required"
        else:
            decision = fraud_pb2.FraudResult.APPROVED
            reason = "OK"

        return fraud_pb2.FraudResult(
            decision=decision, score=score, reason=reason
        )

    def _compute_score(self, request):
        score = 0.0
        if request.amount > self.AMOUNT_THRESHOLD:
            score += 0.4
        # ... additional heuristics
        return min(score, 1.0)
@}
```



### Deployment and Operations (Shell)

Shell scripts handle the operational side of payments: running
migrations, seeding data, and deploying. These scripts live in the
same concept file because they serve the same business concept:

**Migration runner:**

```
@d Payments: run migration @{
#!/usr/bin/env bash
set -euo pipefail

DB_URL="${DATABASE_URL:?DATABASE_URL must be set}"
MIGRATION_FILE="migrations/003_payments.sql"

echo "Running payment migration..."
psql "$DB_URL" -f "$MIGRATION_FILE"
echo "Payment migration complete."
@}
```

**Seed script:**

```
@d Payments: run seeds @{
#!/usr/bin/env bash
set -euo pipefail

DB_URL="${DATABASE_URL:?DATABASE_URL must be set}"
SEED_FILE="seeds/payments.sql"

echo "Seeding payment data..."
psql "$DB_URL" -f "$SEED_FILE"
echo "Payment seed data loaded."
@}
```

**Deploy script** (orchestrates the full deployment for the payments
capability):

```
@d Payments: deploy script @{
#!/usr/bin/env bash
set -euo pipefail

echo "=== Deploying Payments ==="

# 1. Run database migration
@<Payments: run migration@>

# 2. Generate gRPC code from proto
protoc --go_out=. --go-grpc_out=. proto/payments/v1/fraud.proto
protoc --python_out=. --grpc_python_out=. proto/payments/v1/fraud.proto

# 3. Build and deploy Go service
cd go-service && go build -o payment-service ./cmd/server
echo "Go service built."

# 4. Deploy Python fraud detector
cd ../python-fraud && pip install -r requirements.txt
echo "Python fraud detector ready."

echo "=== Payments deployed ==="
@}
```

Notice how the deploy script uses `@<Payments: run migration@>` ---
it references a chunk defined earlier in the same concept file. The
migration code is written once and reused in both the standalone
migration runner and the full deploy script.



## The Assembly: Routing to Multiple Systems

The assembly file routes chunks from `payments.weft` to their physical
locations across multiple systems:

```
% In assembly.weft:

% --- Database ---
@o tmp/go-service/migrations/003_payments.sql @{
@<Payments: migration@>
@}

@o tmp/go-service/seeds/payments.sql @{
@<Payments: seed data@>
@}

% --- GraphQL contract ---
@o tmp/go-service/gql/schema/payment.graphql @{
@<Payments: schema GraphQL@>
@}

% --- Go backend ---
@o tmp/go-service/internal/domain/entity/payment.go @{
package entity

import "time"
@<Payments: Go entity@>
@}

% --- Flutter app ---
@o tmp/flutter-app/lib/features/payments/models/payment.dart @{
@<Payments: Flutter model@>
@}

@o tmp/flutter-app/lib/features/payments/services/payment_service.dart @{
@<Payments: Flutter service@>
@}

@o tmp/flutter-app/lib/features/payments/screens/payment_screen.dart @{
@<Payments: Flutter screen@>
@}

% --- gRPC contract ---
@o tmp/proto/payments/v1/fraud.proto @{
syntax = "proto3";
@<Payments: proto gRPC@>
@}

% --- Python fraud detector ---
@o tmp/python-fraud/src/payments/detector.py @{
@<Payments: Python fraud detector@>
@}

% --- Shell scripts ---
@o tmp/scripts/migrate-payments.sh @{
@<Payments: run migration@>
@}

@o tmp/scripts/seed-payments.sh @{
@<Payments: run seeds@>
@}

@o tmp/scripts/deploy-payments.sh @{
@<Payments: deploy script@>
@}
```

One concept file (`payments.weft`) produces **12 output files** across 5
systems. The assembly file is the only place that knows about physical
paths --- the concept file is pure narrative and logic.



## Multi-System Justfile

The justfile handles post-tangle steps for each technology:

```just
master := "project.weft"

setup:
    mkdir -p tmp/go-service/migrations tmp/go-service/seeds
    mkdir -p tmp/go-service/gql/schema
    mkdir -p tmp/go-service/internal/domain/entity
    mkdir -p tmp/flutter-app/lib/features/payments/models
    mkdir -p tmp/flutter-app/lib/features/payments/services
    mkdir -p tmp/flutter-app/lib/features/payments/screens
    mkdir -p tmp/proto/payments/v1
    mkdir -p tmp/python-fraud/src/payments
    mkdir -p tmp/scripts

tangle: setup
    weft {{master}}

format: tangle
    # Go
    goimports -w tmp/go-service/
    # Dart
    dart format tmp/flutter-app/
    # Python
    black tmp/python-fraud/
    # Shell
    chmod +x tmp/scripts/*.sh

build-go: format
    cd tmp/go-service && go build ./...

build-flutter: format
    cd tmp/flutter-app && flutter pub get && flutter build

build-proto: tangle
    protoc --go_out=. --go-grpc_out=. tmp/proto/payments/v1/*.proto
    protoc --python_out=. --grpc_python_out=. tmp/proto/payments/v1/*.proto

build: build-go build-flutter build-proto

all: tangle format build

clean:
    rm -rf tmp/
```

The key insight: **one `weft` call** produces all outputs for
all systems. Then each system's tools run on their respective outputs.



## What Changes Per Language

The LP structure is always the same. Only these details change:

| Aspect | Go | Flutter/Dart | Python | SQL | Shell | Protobuf |
|--------|------|-------------|--------|-----|-------|----------|
| Output extension | `.go` | `.dart` | `.py` | `.sql` | `.sh` | `.proto` |
| Post-tangle tool | `goimports` | `dart format` | `black` | — | `chmod +x` | `protoc` |
| Build step | `go build` | `flutter build` | — | `psql -f` | — | `protoc` |
| Special escaping | `@key` in GQL | `@override` | `@decorator` | — | — | — |
| Test tool | `go test` | `flutter test` | `pytest` | — | `bats` | — |

**What never changes:**

- One `.weft` per business concept
- Chunks named `Domain: system description`
- Master file with `@i` includes
- Assembly file with all `@o` directives
- Narrative first, code second
- justfile orchestrating the pipeline

The pattern is universal. The languages are parameters.



## Modifying an Existing Concept

The worked example above shows *creating* a concept from scratch. But
most day-to-day work is *modifying* an existing concept. Here's what
that looks like.

### Example: Add `description` field to Payments

The user says: "payments need a description field." You open `payments.weft`
— the one file — and make these changes:

**1. Update the migration** (add ALTER TABLE after the CREATE TABLE):
```
@d Payments: migration add description @{
ALTER TABLE payments ADD COLUMN description TEXT;
@}
```

**2. Update the GraphQL schema** (add field to type and input):
```
# Inside @d Payments: schema GraphQL, add:
description: String
```

**3. Update the Go entity** (add field to struct):
```
# Inside @d Payments: Go entity, add:
Description string
```

**4. Update the Flutter model** (add field to class):
```
# Inside @d Payments: Flutter model, add:
final String? description;
```

**5. Update the narrative** — explain *why* the description exists:
> Merchants requested a free-text description so users can identify
> payments in their history. The field is optional (nullable) because
> existing payments lack it.

**6. Build**: `just all` — one tangle produces updated files across all
5 systems.

### What makes this different from traditional development

In traditional development, the same change touches:
- `migrations/004_add_description.sql`
- `gql/schema/payment.graphql`
- `internal/domain/entity/payment.go`
- `lib/features/payments/models/payment.dart`
- Possibly `payment_service.dart`, `payment_screen.dart`

That's 5+ files in 3+ directories (or repositories). Each file opened
separately. The *reason* for the change exists only in the commit
message or a Jira ticket.

In LP, you open `payments.weft` once. The reason is written as prose. The
code changes are visible together. Consistency across systems is
checked by reading one screenful, not by jumping between files.



## Why This Matters

### 1. Contracts live between their consumers

The GraphQL schema sits between Go (producer) and Flutter (consumer)
in the narrative. The Protobuf definition sits between Go (client) and
Python (server). Any contract change is immediately visible in context
with both sides.

### 2. Consistency is visible

The payment status enum appears in SQL (`payment_status`), GraphQL
(`PaymentStatus`), Go (`PaymentStatus`), Dart (`PaymentStatus`), and
Protobuf (`Decision`). Because all five definitions live in one
narrative, inconsistencies are immediately visible.

### 3. One change, one file

Adding a new field to payments (say, `description`) means opening
`payments.weft` once. You add it to the SQL migration, GraphQL schema, Go
entity, Dart model, and seed data — all in the same editing session,
guided by the narrative.

### 4. The "why" crosses system boundaries

"Why does the Go service call the fraud detector before persisting?"
In traditional code, you'd have to infer this from the code flow. In
the literate program, the narrative explains: "We call fraud detection
first because rejected payments should never touch the database."

### 5. Onboarding is reading

A new developer reads `payments.weft` from top to bottom and understands the
complete payment flow across all systems. No need to trace through 12
files in 3 repositories.
